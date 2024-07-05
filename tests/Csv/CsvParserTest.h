#include "gtest/gtest.h"
#include "Csv/parser/CsvParser.h"
#include "Csv/content/CsvContent.h"
#include <string>
#include <vector>

// NOLINTBEGIN

class CsvParserTest : public::testing::Test {
protected:
	static constexpr CsvProperties_t props = { .has_header = false };
	CsvParser_t *parser;
	void SetUp() override {
		parser = CsvParser_Init(&props);
		ASSERT_NE(nullptr, parser);
	}
	void TearDown() override {
		CsvParser_Destroy(parser);
	}
	void AssertContent(CsvContent_t *expected) {
		CsvContent_t *actual = const_cast<CsvContent_t *>(CsvParser_GetContent(parser));
		ASSERT_EQ(expected->lines.length, actual->lines.length);
		for (size_t i = 0; i < expected->lines.length; i++) {
			CsvLine_t *expected_line = &expected->lines.list[i];
			CsvLine_t *actual_line = &actual->lines.list[i];
			ASSERT_EQ(expected_line->items.length, actual_line->items.length);
			for (size_t j = 0; j < expected_line->items.length; j++) {
				CsvItem_t *expected_item = &expected_line->items.list[j];
				CsvItem_t *actual_item = &actual_line->items.list[j];
				ASSERT_STREQ(expected_item->text, actual_item->text);
			}
		}
	}
};

TEST_F(CsvParserTest, Scenario1) {
	std::string data{
		"title1,title2,title3\n"
		"abc,def,ghi"
	};
	std::vector<CsvItem_t> items0{ {"title1"}, {"title2"}, {"title3"} };
	std::vector<CsvItem_t> items1{ {"abc"}, {"def"}, {"ghi"}, };
	std::vector<CsvLine_t> lines{
		{.items = {.list = items0.data(), .length = items0.size()}},
		{.items = {.list = items1.data(), .length = items1.size()}},
	};
	CsvContent_t content = { {.list = lines.data(), .length = lines.size()} };

	ASSERT_EQ(CSV_SUCCESS, CsvParser_LoadFromData(parser, data.c_str(), data.length()));

	AssertContent(&content);
}

TEST_F(CsvParserTest, Scenario2) {
	std::string data{
		"title1,title2\r\n"
		"data1,data2"
	};
	std::vector<CsvItem_t> items0{ {"title1"}, {"title2"} };
	std::vector<CsvItem_t> items1{ {"data1"}, {"data2"} };
	std::vector<CsvLine_t> lines{
		{.items = {.list = items0.data(), .length = items0.size()}},
		{.items = {.list = items1.data(), .length = items1.size()}},
	};
	CsvContent_t content = { {.list = lines.data(), .length = lines.size()} };

	ASSERT_EQ(CSV_SUCCESS, CsvParser_LoadFromData(parser, data.c_str(), data.length()));
	AssertContent(&content);
}

TEST_F(CsvParserTest, Scenario3) {
	std::string data{
		"title1,title2,title3\n"
		"data1"
	};
	std::vector<CsvItem_t> items0{ {"title1"}, {"title2"}, {"title3"} };
	std::vector<CsvItem_t> items1{ {"data1"} };
	std::vector<CsvLine_t> lines{
		{.items = {.list = items0.data(), .length = items0.size()}},
		{.items = {.list = items1.data(), .length = items1.size()}},
	};
	CsvContent_t content = { {.list = lines.data(), .length = lines.size()} };

	ASSERT_EQ(CSV_SUCCESS, CsvParser_LoadFromData(parser, data.c_str(), data.length()));
	AssertContent(&content);
}

TEST_F(CsvParserTest, Scenario4) {
	std::string data{ "" };
	std::vector<CsvItem_t> items{ {""} };
	std::vector<CsvLine_t> lines{
		{{.list = items.data(), .length = items.size()}}
	};
	CsvContent_t content = { {.list = lines.data(), .length = lines.size()} };

	ASSERT_EQ(CSV_INVALID_PARAMETER, CsvParser_LoadFromData(parser, data.c_str(), data.length()));
	// AssertContent(&content);
}

TEST_F(CsvParserTest, Scenario5) {
	/**
	 * input.csv
	 *  scenario5\r\n
	 *  title1,title2\r\n
	 *  data1,data2\r\n
	 *  EOF
	*/
	std::vector<CsvItem_t> items0{ {"scenario5"} };
	std::vector<CsvItem_t> items1{ {"title1"}, {"title2"} };
	std::vector<CsvItem_t> items2{ {"data1"}, {"data2"} };
	std::vector<CsvLine_t> lines{
		{{.list = items0.data(), .length = items0.size()}},
		{{.list = items1.data(), .length = items1.size()}},
		{{.list = items2.data(), .length = items2.size()}},
	};
	CsvContent_t content{ {.list = lines.data(), .length = lines.size()} };
	ASSERT_EQ(CSV_SUCCESS, CsvParser_LoadFromFile(parser, "/home/atohs/C/CUtils/tests/Csv/input.csv"));
	AssertContent(&content);
}

TEST_F(CsvParserTest, Scenario6) {
	std::string data{
		"タイトル1,title2"
		"データ1,data2"
	};
	ASSERT_EQ(CSV_INVALID_CHAR_CODE, CsvParser_LoadFromData(parser, data.c_str(), data.length()));
}

// NOLINTEND
