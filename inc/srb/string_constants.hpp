/**
 * Copyright 2018 GLUG-NITH
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef SRB_STRING_CONSTANTS_HPP
#define SRB_STRING_CONSTANTS_HPP

#include <string>

namespace srb
{
  namespace cheema
  {
    const char STDIN[] = "stdin";
    const char STDOUT[] = "stdout";
    const char WARNING_LEVEL[] = "warning";
    const char ERROR_LEVEL[] = "error";
    const char ERRORS[] = "errors";
    const char STOP_LEVEL[] = "stop";
    const char HELP[] = "help";
    const char VERSION[] = "version";
    const char LEVEL[] = "level";
    const char DATABASE[] = "database";
    const char TEXT[] = "text";
    const char SUMMARY[] = "summary";
    const char VALID[] = "valid";
    const char INPUT[] = "input";
    const char VCF[] = "vcf";
    const char OUTPUT[] = "output";
    const char OUTDIR[] = "outdir";
    const char REPORT[] = "report";
    const char HELP_OPTION[] = "help,h";
    const char VERSION_OPTION[] = "version,v";
    const char INPUT_OPTION[] = "input,i";
    const char ERRORS_OPTION[] = "errors,e";
    const char LEVEL_OPTION[] = "level,l";
    const char REPORT_OPTION[] = "report,r";
    const char OUTDIR_OPTION[] = "outdir,o";
    const char OUTPUT_OPTION[] = "output,o";

    // Compressed file extensions
    const std::string BZ2 = ".bz2";
    const std::string NO_EXT = "";
    const std::string RAR = ".rar";
    const std::string TAR = ".tar";
    const std::string GZ = ".gz";
    const std::string XZ = ".xz";
    const std::string Z = ".Z";
    const std::string ZIP = ".zip";
    const std::string ZLIB = ".z";
  }
}

#endif // SRB_STRING_CONSTANTS_HPP
