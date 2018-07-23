#include "DNDFile_imp.h"
#include "DNDDebug.h"
#include <fstream>
#include "DNDDebug.h"
#include "DNDValue.h"

namespace DND
{
	File* File::Create()
	{
		File* file = new File_imp();
		return file;
	}





	bool File_imp::OpenFile(const String& path)
	{
		m_path = path;
		wifstream in;
		//in.imbue(locale(""));
		in.open(path.GetWcs(), ios::in);

		if (!in)
		{
			debug_warn(String(L"DND: ���ļ�ʧ��") + path);
			return false;
		}
		

		wchar_t temp[DND_FILE_DATA_LENGTH_MAX + 2];
		String str_name;
		String str_value;
		in >> temp;
		if (wcscmp(L"[DNDFILE]", temp) == 0)
		{

			//////////////////////////////////////////////////////////////////////////����node

			for (int i = 0;; i++)
			{

				temp[0] = 0;
				in >> temp;
				if (wcscmp(temp, L"[-]") == 0)
				{
					//assert(0 && L"DNDFile ��ȡ������");
					in.close();
					return true;
				}
				else if (temp[0] != 0)
					str_name = temp;
				else
				{
					in.close();
					return false;
				}

				temp[0] = 0;
				in >> temp;
				if (wcscmp(temp, L"=") != 0)
				{
					debug_warn(String(L"DND: Fileƥ��Ⱥ�ʧ��: ") + path + L": ��" + String(i) + L"��ֵ");
					in.close();
					return false;
				}

				temp[0] = 0;
				in >> temp;
				if (temp[0] != 0)
					str_value = temp;
				else
				{
					debug_warn(String(L"DND: Fileƥ��ֵʧ��: ") + path + L": ��" + String(i) + L"��ֵ");
					in.close();

					return false;
				}

				m_strings[str_name] = str_value;

			}

		}
		else//��һ�б�������I���
		{
			debug_warn(String(L"DND: Fileƥ������ʧ��: ") + path);
			in.close();
			return false;
		}
	}



	const String& File_imp::GetValue(const String& name)
	{
		if (m_strings.count(name) > 0)
			return m_strings[name];
		return DEAULT_EMPTY_STRING;
	}

	const DND::String& File_imp::GetValue(UINT32 i)
	{
		auto& iter = m_strings.begin();

		if (i == 0)
			return iter->second;

		while (i--)
		{
			iter++;
		}

		return iter->second;
	}

	void File_imp::CreateFile(const String& path)
	{
		m_path = path;
		wofstream out;
		//out.imbue(locale(""));
		out.open(path.GetWcs(), ios::out | ios::trunc);

		if (!out)
		{
			debug_warn(String(L"DND: File����ʧ��: ") + path);
		}
	}

	void File_imp::SetValue(const String& name, const String& value)
	{
		m_strings[name] = value;
	}

	void File_imp::Save()
	{
		wofstream out;
		out.imbue(locale(""));
		out.open(m_path.GetWcs(), ios::out);

		out << L"[DNDFILE]" << endl;



		map<String, String>::iterator itor = m_strings.begin();
		for (; itor != m_strings.end(); ++itor)
		{
			out << itor->first.GetWcs() << L" = " << itor->second.GetWcs() << endl;
		}

		out << L"[-]";
	}


	const DND::String& File_imp::GetKey(UINT32 i)
	{
		auto& iter = m_strings.begin();

		if (i == 0)
			return iter->first;

		while (i--)
		{
			iter++;
		}

		return iter->first;

	}

	UINT32 File_imp::GetLength()
	{
		return m_strings.size();
	}

}

