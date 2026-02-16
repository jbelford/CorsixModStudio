/*
Rainman Library
Copyright (C) 2006 Corsix <corsix@gmail.com>

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef _RAINMAN_LOG_H_
#define _RAINMAN_LOG_H_

#include "rainman/core/Api.h"

#include <spdlog/spdlog.h>
#include <memory>

//! Centralised logging facility for the Rainman library.
/*!
    Call RainmanLog::init() once at application start-up (before any I/O).
    After that, use the convenience macros below — they resolve to spdlog
    calls on the "rainman" named logger.

    A second logger named "cdms" is also created for the GUI layer; retrieve
    it with RainmanLog::getCdms().
*/
class RAINMAN_API RainmanLog
{
  public:
	//! Initialise console + rotating-file sinks.
	/*!
	    Safe to call more than once (subsequent calls are no-ops).
	    \param[in] sLogDir  Directory for log files (default: "logs")
	*/
	static void init(const char *sLogDir = "logs");

	//! Return the "rainman" logger (library layer).
	static std::shared_ptr<spdlog::logger> get();

	//! Return the "cdms" logger (GUI layer).
	static std::shared_ptr<spdlog::logger> getCdms();

  private:
	static bool s_bInitialised;
};

// ---------------------------------------------------------------------------
// Convenience macros — zero overhead when compiled out via SPDLOG_ACTIVE_LEVEL
// ---------------------------------------------------------------------------
#define RAINMAN_LOG_TRACE(...) SPDLOG_LOGGER_TRACE(RainmanLog::get(), __VA_ARGS__)
#define RAINMAN_LOG_DEBUG(...) SPDLOG_LOGGER_DEBUG(RainmanLog::get(), __VA_ARGS__)
#define RAINMAN_LOG_INFO(...) SPDLOG_LOGGER_INFO(RainmanLog::get(), __VA_ARGS__)
#define RAINMAN_LOG_WARN(...) SPDLOG_LOGGER_WARN(RainmanLog::get(), __VA_ARGS__)
#define RAINMAN_LOG_ERROR(...) SPDLOG_LOGGER_ERROR(RainmanLog::get(), __VA_ARGS__)
#define RAINMAN_LOG_CRITICAL(...) SPDLOG_LOGGER_CRITICAL(RainmanLog::get(), __VA_ARGS__)

#define CDMS_LOG_TRACE(...) SPDLOG_LOGGER_TRACE(RainmanLog::getCdms(), __VA_ARGS__)
#define CDMS_LOG_DEBUG(...) SPDLOG_LOGGER_DEBUG(RainmanLog::getCdms(), __VA_ARGS__)
#define CDMS_LOG_INFO(...) SPDLOG_LOGGER_INFO(RainmanLog::getCdms(), __VA_ARGS__)
#define CDMS_LOG_WARN(...) SPDLOG_LOGGER_WARN(RainmanLog::getCdms(), __VA_ARGS__)
#define CDMS_LOG_ERROR(...) SPDLOG_LOGGER_ERROR(RainmanLog::getCdms(), __VA_ARGS__)
#define CDMS_LOG_CRITICAL(...) SPDLOG_LOGGER_CRITICAL(RainmanLog::getCdms(), __VA_ARGS__)

#endif // _RAINMAN_LOG_H_
