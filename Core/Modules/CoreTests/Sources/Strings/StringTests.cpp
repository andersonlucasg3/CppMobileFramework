#include "StringTests.h"

#include "Assert.h"
#include "String/String.h"

REGISTER_TEST_CLASS(StringTests);

void CStringTests::TestCase()
{
	// Test basic ASCII replace
	CString Rep1 = CString("abc123abc").Replace("abc", "xyz");
	ASSERT_EQUAL(Rep1, "xyz123xyz");

	// Test chained replace
	CString Rep2 = Rep1.Replace("xyz", "hello");
	ASSERT_EQUAL(Rep2, "hello123hello");

	// Test replace with longer string
	CString Rep3 = CString("fox").Replace("o", "ooo");
	ASSERT_EQUAL(Rep3, "fooox");

	// Test replace shrinking
	CString Rep4 = Rep3.Replace("ooo", "o");
	ASSERT_EQUAL(Rep4, "fox");

	// Test UTF-8 accented characters
	CString Str5 = "Eu s\xc3\xb3 quero meter o p\xc3\xa9 porran";
	CString Rep5 = Str5.Replace("\xc3\xb3", "o").Replace("\xc3\xa9", "e");
	ASSERT_EQUAL(Rep5, "Eu so quero meter o pe porran");

	// Test chained replace round-trip
	CString Rep6 = Rep5.Replace("o", "sair daqui");
	ASSERT_EQUAL(Rep6, "Eu ssair daqui quersair daqui meter sair daqui pe psair daquirran");

	CString Rep7 = Rep6.Replace("sair daqui", "o");
	Rep7 = Rep7.Replace("so", "s\xc3\xb3");
	Rep7 = Rep7.Replace("pe", "p\xc3\xa9");
	ASSERT_EQUAL(Rep7, "Eu s\xc3\xb3 quero meter o p\xc3\xa9 porran");
}