#include "StringTests.h"

#include "Assert.h"
#include "String/String.h"

REGISTER_TEST_CLASS(StringTests);

const char* const CStrArray[10] {
	"First string test 1",
	"First string test 2",
	"First string test 3",
	"First string test 4",
	"First string test 5",
	"First string test 6",
	"First string test 7",
	"First string test 8",
	"First string test 9",
	"First string test 10"
};

void CStringTests::TestCase()
{
    CString String = "Eu só quero meter o pé porran";

	CString Replaced = String.Replace("ó", "o").Replace("é", "e");

	ASSERT_EQUAL(Replaced, "Eu so quero meter o pe porran");

	Replaced = Replaced.Replace("o", "sair daqui");

	ASSERT_EQUAL(Replaced, "Eu ssair daqui quersair daqui meter sair daqui pe psair daquirran");

	Replaced = Replaced.Replace("sair daqui", "o").Replace("so", "só");
	Replaced = Replaced.Replace("pe", "pé");

	ASSERT_EQUAL(Replaced, "Eu só quero meter o pé porran");
}