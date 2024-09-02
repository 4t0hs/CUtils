#include <glib-2.0/glib.h>
#include <pthread.h>
#include <wait.h>
#include <signal.h>
#include <stdatomic.h>
#include <stdio.h>
#include "utilities.h"
#include "ProcessManager.h"

typedef struct ProcessStartInfo {
	pid_t pid;
	void *data;
} ProcessStartInfo;

typedef struct ProcessManager {
	GHashTable *processes;
	pthread_mutex_t mutex;
	ProcessExitedEventHandler onExited;
} ProcessManager;

#define INITIALIZE_NUM_PROCESSES	64
#define LOCK(m, CODE) { pthread_mutex_lock((m)); CODE; pthread_mutex_unlock((m)); }

static ProcessManager _processManager;
static atomic_bool initialized = false;
static inline ProcessManager *getMyself() {
	return &_processManager;
}

static gboolean AreEquals(pid_t a, pid_t b) {
	return a == b;
}

static guint GenerateHash(pid_t pid) {
	return pid;
}

static void sigchldHandler(int signalNumber, siginfo_t *signalInfo, void *ucontext) {
	ProcessManager *self = getMyself();
	void *data;
	LOCK(&self->mutex, {
		data = g_hash_table_lookup(self->processes, (gconstpointer)signalInfo->si_pid);
		});
	// printf("Process %d exited with code %d\n", signalInfo->si_pid, signalInfo->si_status);
	if (!data || !self->onExited) {
		return;
	}
	if (self->onExited) {
		self->onExited(signalInfo->si_pid, signalInfo->si_status, data);
	}
}

static void HandleSigchld() {
	struct sigaction sa;
	sigemptyset(&sa.sa_mask);
	sa.sa_sigaction = sigchldHandler;
	sa.sa_flags = SA_SIGINFO;
	sigaction(SIGCHLD, &sa, NULL);
}

void ProcessManager_Init(ProcessExitedEventHandler onExited) {
	if (initialized) return;
	initialized = true;
	ProcessManager *self = getMyself();
	CLEAR(self);
	self->onExited = onExited;
	self->processes = g_hash_table_new((GHashFunc)GenerateHash, (GEqualFunc)AreEquals);
	pthread_mutex_init(&self->mutex, NULL);
	HandleSigchld();
}

void ProcessManager_Destroy() {
	ProcessManager *self = getMyself();
	g_hash_table_destroy(self->processes);
	CLEAR(self);
}

void ProcessManager_Register(pid_t pid, void *data) {
	ProcessManager *self = getMyself();
	LOCK(&self->mutex, {
		g_hash_table_insert(self->processes, (gpointer)pid, data);
		});
}

void ProcessManager_Unregister(pid_t pid) {
	ProcessManager *self = getMyself();
	LOCK(&self->mutex, {
		g_hash_table_remove(self->processes, (gconstpointer)pid);
		});
	return;
}
