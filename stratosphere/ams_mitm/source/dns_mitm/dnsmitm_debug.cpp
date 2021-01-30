/*
 * Copyright (c) 2018-2020 Atmosphère-NX
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <stratosphere.hpp>
#include "../amsmitm_fs_utils.hpp"
#include "dnsmitm_debug.hpp"

namespace ams::mitm::socket::resolver {

    #if 1

    namespace {

        ::FsFile g_log_file;
        s64 g_log_ofs;

        os::SdkMutex g_log_mutex;

        char g_log_buf[0x400];

    }

    void InitializeDebug() {
        /* Create the log file. */
        mitm::fs::CreateAtmosphereSdFile("dns_log.txt", 0, ams::fs::CreateOption_None);

        /* Open the log file. */
        R_ABORT_UNLESS(mitm::fs::OpenAtmosphereSdFile(std::addressof(g_log_file), "dns_log.txt", ams::fs::OpenMode_ReadWrite | ams::fs::OpenMode_AllowAppend));

        /* Get the current log offset. */
        R_ABORT_UNLESS(::fsFileGetSize(std::addressof(g_log_file), std::addressof(g_log_ofs)));

        /* Start a new log. */
        LogDebug("\n---\n");
    }

    void LogDebug(const char *fmt, ...) {
        std::scoped_lock lk(g_log_mutex);

        int len = 0;
        {
            std::va_list vl;
            va_start(vl, fmt);
            len = util::VSNPrintf(g_log_buf, sizeof(g_log_buf), fmt, vl);
            va_end(vl);
        }

        R_ABORT_UNLESS(::fsFileWrite(std::addressof(g_log_file), g_log_ofs, g_log_buf, len, FsWriteOption_Flush));
        g_log_ofs += len;
    }

    #else

    void InitializeDebug() {
        /* ... */
    }

    void LogDebug(const char *fmt, ...) {
        /* ... */
    }

    #endif

}
