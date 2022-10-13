#define FMT_HEADER_ONLY
#include <fstream>

#include "fmt/format.h"

#include "IniSave.h"
#include "IniFile.h"

namespace HawtLib {
	namespace File {
		namespace Persistence {
			void IniSave(IniFile& iniFile, const std::string& file) {
				std::ofstream ofs(file);
				for (std::string sectionName : iniFile.GetSectionNames()) {
					ofs << fmt::format("[{}]", sectionName) << std::endl;
					auto keyValues = iniFile.GetSectionKV(sectionName)->keyValues;
					for (auto kv : keyValues) {
						ofs << fmt::format("{} = {}", kv->key, kv->value) << std::endl;
					}
					ofs << std::endl;
				}
				ofs.close();
			}
		}
	}
}
