// https://www.codeproject.com/Articles/8628/Retrieving-version-information-from-your-local-app
// GlobalFunctions.h: Schnittstelle für die Klasse CGlobalFunctions.
// luetz
// CEO
// Switzerland Switzerland
//////////////////////////////////////////////////////////////////////
#pragma once

namespace version
{


   string get_version();

   int compare(string str1, string str2);



} // namespace version


class Version
{
public:

   inline static bool le(const string & str1, const string & str2) {return ::version::compare(str1, str2) <= 0;}

};
