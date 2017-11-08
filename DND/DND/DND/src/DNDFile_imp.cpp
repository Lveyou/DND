#include "DNDFile_imp.h"
#include "DNDDebug.h"
#include <fstream>
#include "DNDDebug.h"

namespace DND
{
	File* File::Create()
	{
		File* file = new File_imp();
		return file;
	}

	void File::Copy(const String& source, const String& target)
	{
		CopyFile(source.GetWcs(), target.GetWcs(), FALSE);
		
	}


	HANDLE File::m_find = NULL;
	WIN32_FIND_DATA File::m_find_data;

	bool File::GetPathFileFirst(const String& path, String& name)
	{

		m_find = ::FindFirstFile(path.GetWcs(), &m_find_data);

		if (INVALID_HANDLE_VALUE == m_find)
		{
			return false;
		}

		name = m_find_data.cFileName;
		return true;
	}

	bool File::GetPathFileNext(const String& path, String& name)
	{
		while (FindNextFile(m_find, &m_find_data))
		{
			name = m_find_data.cFileName;
			return true;
		}
		return false;
	}

	void File_imp::OpenFile(const String& path)
	{
		m_path = path;
		wifstream in;
		//in.imbue(locale(""));
		in.open(path.GetWcs(), ios::in);

		if (!in)
		{
			debug_warn(String(L"DND: 打开文件失败") + path);
			return;
		}
		

		wchar_t temp[DND_FILE_DATA_LENGTH_MAX + 2];
		String str_name;
		String str_value;
		in >> temp;
		if (wcscmp(L"[DNDFILE]", temp) == 0)
		{

			//////////////////////////////////////////////////////////////////////////处理node

			for (int i = 0;; i++)
			{

				temp[0] = 0;
				in >> temp;
				if (wcscmp(temp, L"[-]") == 0)
				{
					//assert(0 && L"DNDFile 读取结束！");
					in.close();
					return;
				}
				else if (temp[0] != 0)
					str_name = temp;
				else
				{
					in.close();
					return;
				}

				temp[0] = 0;
				in >> temp;
				if (wcscmp(temp, L"=") != 0)
				{
					debug_warn(String(L"DND: File匹配等号失败: ") + path + L": 第" + String(i) + L"个值");
					in.close();
					return;
				}

				temp[0] = 0;
				in >> temp;
				if (temp[0] != 0)
					str_value = temp;
				else
				{
					debug_warn(String(L"DND: File匹配值失败: ") + path + L": 第" + String(i) + L"个值");
					in.close();

					return;
				}

				m_strings[str_name] = str_value;

			}

		}
		else//第一行必须这样I标记
		{
			debug_warn(String(L"DND: File匹配首行失败: ") + path);
			in.close();
		}
	}

	String File_imp::GetValue(const String& name)
	{
		if (m_strings.count(name) > 0)
			return m_strings[name];
		return L"";
	}

	void File_imp::CreateFile(const String& path)
	{
		m_path = path;
		wofstream out;
		out.imbue(locale(""));
		out.open(path.GetWcs(), ios::out);

		if (!out)
		{
			debug_warn(String(L"DND: File创建失败: ") + path);
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


}

