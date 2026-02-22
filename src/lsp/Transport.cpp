/*
LSP Client Library
Copyright (C) 2026 CorsixModStudio Contributors

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

#include "lsp/Transport.h"
#include <rainman/core/RainmanLog.h>

namespace lsp
{

CProcess::~CProcess() { Cleanup(); }

bool CProcess::Start(const std::wstring &exePath, const std::wstring &args, const std::wstring &workingDir)
{
    Cleanup();

    // Create pipes for stdin/stdout redirection
    SECURITY_ATTRIBUTES sa{};
    sa.nLength = sizeof(sa);
    sa.bInheritHandle = TRUE;
    sa.lpSecurityDescriptor = nullptr;

    HANDLE hStdinRead = INVALID_HANDLE_VALUE;
    HANDLE hStdoutWrite = INVALID_HANDLE_VALUE;

    if (!CreatePipe(&hStdinRead, &m_hStdinWrite, &sa, 0))
    {
        return false;
    }

    if (!CreatePipe(&m_hStdoutRead, &hStdoutWrite, &sa, 0))
    {
        CloseHandle(hStdinRead);
        CloseHandle(m_hStdinWrite);
        m_hStdinWrite = INVALID_HANDLE_VALUE;
        return false;
    }

    // Ensure our ends of the pipes are not inherited by the child
    SetHandleInformation(m_hStdinWrite, HANDLE_FLAG_INHERIT, 0);
    SetHandleInformation(m_hStdoutRead, HANDLE_FLAG_INHERIT, 0);

    // Build command line: "exePath" args
    std::wstring cmdLine = L"\"" + exePath + L"\"";
    if (!args.empty())
    {
        cmdLine += L" " + args;
    }

    STARTUPINFOW si{};
    si.cb = sizeof(si);
    si.dwFlags = STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
    si.hStdInput = hStdinRead;
    si.hStdOutput = hStdoutWrite;
    si.hStdError = hStdoutWrite;
    si.wShowWindow = SW_HIDE;

    PROCESS_INFORMATION pi{};

    BOOL ok = CreateProcessW(nullptr,
                             cmdLine.data(), // mutable command line required by CreateProcessW
                             nullptr,        // process security
                             nullptr,        // thread security
                             TRUE,           // inherit handles
                             CREATE_NO_WINDOW,
                             nullptr, // environment
                             workingDir.empty() ? nullptr : workingDir.c_str(), &si, &pi);

    // Close the child's ends of the pipes (we don't use them)
    CloseHandle(hStdinRead);
    CloseHandle(hStdoutWrite);

    if (!ok)
    {
        DWORD err = GetLastError();
        CDMS_LOG_ERROR("LSP: CreateProcess failed (error={})", err);
        CloseHandle(m_hStdinWrite);
        CloseHandle(m_hStdoutRead);
        m_hStdinWrite = INVALID_HANDLE_VALUE;
        m_hStdoutRead = INVALID_HANDLE_VALUE;
        return false;
    }

    CDMS_LOG_INFO("LSP: Process created (PID={})", pi.dwProcessId);
    m_hProcess = pi.hProcess;
    CloseHandle(pi.hThread);

    return true;
}

bool CProcess::Write(const std::string &data) { return Write(data.data(), data.size()); }

bool CProcess::Write(const char *data, size_t length)
{
    if (m_hStdinWrite == INVALID_HANDLE_VALUE)
    {
        return false;
    }

    size_t totalWritten = 0;
    while (totalWritten < length)
    {
        DWORD written = 0;
        if (!WriteFile(m_hStdinWrite, data + totalWritten, static_cast<DWORD>(length - totalWritten), &written,
                       nullptr))
        {
            return false;
        }
        totalWritten += written;
    }
    return true;
}

std::string CProcess::Read(size_t bufferSize)
{
    if (m_hStdoutRead == INVALID_HANDLE_VALUE)
    {
        return {};
    }

    // Check if data is available without blocking
    DWORD available = 0;
    if (!PeekNamedPipe(m_hStdoutRead, nullptr, 0, nullptr, &available, nullptr) || available == 0)
    {
        return {};
    }

    size_t toRead = (std::min)(static_cast<size_t>(available), bufferSize);
    std::string result(toRead, '\0');

    DWORD bytesRead = 0;
    if (!ReadFile(m_hStdoutRead, result.data(), static_cast<DWORD>(toRead), &bytesRead, nullptr))
    {
        return {};
    }

    result.resize(bytesRead);
    return result;
}

std::string CProcess::ReadWithTimeout(DWORD timeoutMs, size_t bufferSize)
{
    if (m_hStdoutRead == INVALID_HANDLE_VALUE)
    {
        return {};
    }

    // Wait for the pipe handle to become signalled (data available)
    DWORD waitResult = WaitForSingleObject(m_hStdoutRead, timeoutMs);
    if (waitResult != WAIT_OBJECT_0)
    {
        return {};
    }

    // Data should be available — do a non-blocking read
    return Read(bufferSize);
}

bool CProcess::IsRunning() const
{
    if (m_hProcess == INVALID_HANDLE_VALUE)
    {
        return false;
    }

    DWORD exitCode = 0;
    if (!GetExitCodeProcess(m_hProcess, &exitCode))
    {
        return false;
    }

    return exitCode == STILL_ACTIVE;
}

void CProcess::Kill()
{
    if (m_hProcess != INVALID_HANDLE_VALUE && IsRunning())
    {
        TerminateProcess(m_hProcess, 1);
        WaitForSingleObject(m_hProcess, 3000);
    }
    Cleanup();
}

void CProcess::Cleanup()
{
    if (m_hStdinWrite != INVALID_HANDLE_VALUE)
    {
        CloseHandle(m_hStdinWrite);
        m_hStdinWrite = INVALID_HANDLE_VALUE;
    }
    if (m_hStdoutRead != INVALID_HANDLE_VALUE)
    {
        CloseHandle(m_hStdoutRead);
        m_hStdoutRead = INVALID_HANDLE_VALUE;
    }
    if (m_hProcess != INVALID_HANDLE_VALUE)
    {
        CloseHandle(m_hProcess);
        m_hProcess = INVALID_HANDLE_VALUE;
    }
}

} // namespace lsp
