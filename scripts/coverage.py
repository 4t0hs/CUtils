#! /usr/bin/python3

import os
import shutil
import subprocess
import argparse
from typing import Any
from pathlib import Path


def ParseArguments() -> Any:
    parser = argparse.ArgumentParser("Generate coverage")
    parser.add_argument("target", help="Test target.")
    return parser.parse_args()


def main():
    arguments = ParseArguments()
    target = arguments.target
    includeDirectories = [
        Path(
            "/home/atohs/C/CUtils/build/lib/CMakeFiles/CUtils.dir",
            target,
        ),
        Path(
            "/home/atohs/C/CUtils/build/tests",
            target,
            "CMakeFiles",
            f"{target}Test.dir",
        ),
    ]
    for d in includeDirectories:
        if not d.exists():
            print(f"{d.as_posix()} does not exist.")
            exit(1)
    output = Path(os.path.join("/home/atohs/C/CUtils/tests", target))
    if not output.exists():
        print(f"{output} does not exist.")
        exit(1)
    # lcov
    print("-------------------------------------")
    print("LCOV")
    print("-------------------------------------")
    coverage = CollectCoverage(tuple(includeDirectories), output)
    # genhtml
    print("-------------------------------------")
    print("GENHTML")
    print("-------------------------------------")
    GenerateHtml(coverage, output)


def CollectCoverage(
    includeDirectories: tuple[Path, ...], outputDirectory: Path
) -> Path:
    tmp = Path(outputDirectory, "tmp_coverage.info")

    if tmp.exists():
        os.remove(tmp)

    command = ["lcov", "-c"]
    for d in includeDirectories:
        command.extend(["-d", d.as_posix()])
        for sub in GetChildDirectories(d):
            command.extend(["-d", sub.as_posix()])
    command.extend(["-o", tmp.as_posix()])
    subprocess.run(command, check=True)

    coverage = Path(outputDirectory, "coverage.info")
    command = [
        "lcov",
        "--remove",
        tmp.as_posix(),
        "*googletest/*",
        "/usr/include/*",
        "-o",
        coverage.as_posix(),
    ]
    subprocess.run(command, check=True)
    os.remove(tmp)
    return coverage


def GetChildDirectories(directory: Path) -> list[Path]:
    result: list[Path] = []
    for c in directory.rglob("*"):
        if c.is_dir():
            result.append(c)
    return result


def GenerateHtml(coverage: Path, output: Path) -> None:
    directory = output.joinpath("HTML")
    if directory.exists():
        shutil.rmtree(directory)
    os.mkdir(directory)

    subprocess.run(
        ["genhtml", "-o", directory.as_posix(), coverage.as_posix()], check=True
    )


if __name__ == "__main__":
    main()
