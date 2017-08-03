/*	EQEMu: Everquest Server Emulator
	Copyright (C) 2001-2015 EQEMu Development Team (http://eqemulator.net)

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; version 2 of the License.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY except by those people which sell it, which
	are required to give you total support for your newly bought product;
	without even the implied warranty of MERCHANTABILITY or FITNESS FOR
	A PARTICULAR PURPOSE. See the GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
*/

#include "stdafx.h"

#include "eqemu_logsys.h"


#include <iostream>
#include <fstream>
#include <string>

#include <direct.h>
#include <iostream>
#include <windows.h>

#pragma once

std::ofstream process_log;

/* Linux ANSI console color defines */
#define LC_RESET   "\033[0m"
#define LC_BLACK   "\033[30m"      /* Black */
#define LC_RED     "\033[31m"      /* Red */
#define LC_GREEN   "\033[32m"      /* Green */
#define LC_YELLOW  "\033[33m"      /* Yellow */
#define LC_BLUE    "\033[34m"      /* Blue */
#define LC_MAGENTA "\033[35m"      /* Magenta */
#define LC_CYAN    "\033[36m"      /* Cyan */
#define LC_WHITE   "\033[37m"      /* White */

namespace Console {
	enum Color {
		Black = 0,
		Blue = 1,
		Green = 2,
		Cyan = 3,
		Red = 4,
		Magenta = 5,
		Brown = 6,
		LightGray = 7,
		DarkGray = 8,
		LightBlue = 9,
		LightGreen = 10,
		LightCyan = 11,
		LightRed = 12,
		LightMagenta = 13,
		Yellow = 14,
		White = 15
	};
}

EQEmuLogSys::EQEmuLogSys()
{
	on_log_gmsay_hook = [](uint16 log_type, const std::string&) {};
	bool file_logs_enabled = false;
	int log_platform = 0;
	
}

EQEmuLogSys::~EQEmuLogSys()
{
}

void EQEmuLogSys::LoadLogSettingsDefaults()
{
	memset(log_settings, 0, sizeof(LogSettings) * Logs::LogCategory::MaxCategoryID);

	/* Set Defaults */
	log_settings[Logs::MysqlErro].log_to_console = Logs::General;
	log_settings[Logs::Normal].log_to_console = Logs::General;
	log_settings[Logs::Setting].log_to_console = Logs::General;
	log_settings[Logs::Files].log_to_console = Logs::General;

	log_settings[Logs::MysqlErro].log_to_file = Logs::Wranging;
	log_settings[Logs::Normal].log_to_file = Logs::Error;
	log_settings[Logs::Setting].log_to_file = Logs::Error;
	log_settings[Logs::Files].log_to_file = Logs::Wranging;


	log_settings[Logs::MysqlErro].is_category_enabled = 1;
	log_settings[Logs::Normal].is_category_enabled = 1;
	log_settings[Logs::Setting].is_category_enabled = 1;

	logbox = (CEdit*)maindlg->GetDlgItem(IDC_LOG);
	logbox->SetLimitText(1000000);


	platform_file_name = "eqrestore";

}

std::string EQEmuLogSys::FormatOutMessageString(uint16 debug_level, uint16 log_category, const std::string &in_message)
{
	std::string ret;

	ret.push_back('[');
	ret.append(Logs::DebugLevelName[debug_level]);
	ret.push_back(']');
	ret.push_back(' ');

	ret.push_back('[');
	ret.append(Logs::LogCategoryName[log_category]);
	ret.push_back(']');
	ret.push_back(' ');
	ret.append(in_message);
	return ret;
}

/*
void EQEmuLogSys::ProcessGMSay(uint16 debug_level, uint16 log_category, const std::string &message)
{
	// Enabling Netcode based GMSay output creates a feedback loop that ultimately ends in a crash 
	if (log_category == Logs::LogCategory::Netcode)
		return;

	// Check to see if the process that actually ran this is zone 
	//if (EQEmuLogSys::log_platform == EQEmuExePlatform::ExePlatformZone)
		on_log_gmsay_hook(log_category, message);
}
*/

void EQEmuLogSys::ProcessLogWrite(uint16 debug_level, uint16 log_category, const std::string &message)
{
	if (log_category == Logs::MysqlErro|| log_category == Logs::Files||Logs::Setting) {
		char time_stamp[80];
		EQEmuLogSys::SetCurrentTimeStamp(time_stamp);
		std::ofstream log;
		EQEmuLogSys::MakeDirectory("logs/");
		log.open(StringFormat("logs/log_%s_%i.log", platform_file_name.c_str(), _getpid()), std::ios_base::app | std::ios_base::out);
		log << time_stamp << " " << message << "\r\n";
		log.close();
	}

	char time_stamp[80];
	EQEmuLogSys::SetCurrentTimeStamp(time_stamp);

	if (process_log)
		process_log << time_stamp << " " << message << std::endl;
}


uint16 EQEmuLogSys::GetWindowsConsoleColorFromCategory(uint16 log_category) {
	switch (log_category) {
		case Logs::Normal:
			return Console::Color::LightGreen;
		case Logs::MysqlErro:
			return Console::Color::LightRed;
		case Logs::Setting:
			return Console::Color::LightRed;
		default:
			return Console::Color::LightGreen;
	}
}

/*std::string EQEmuLogSys::GetLinuxConsoleColorFromCategory(uint16 log_category) {
	switch (log_category) {
		case Logs::Status:
		case Logs::Normal:
			return LC_YELLOW;
		case Logs::MySQLError:
		case Logs::Error:
			return LC_RED;
		case Logs::MySQLQuery:
		case Logs::Debug:
			return LC_GREEN;
		case Logs::Quests:
			return LC_CYAN;
		case Logs::Commands:
		case Logs::Mercenaries:
			return LC_MAGENTA;
		case Logs::Crash:
			return LC_RED;
		default:
			return LC_YELLOW;
	}
}
*/

const std::string EQEmuLogSys::StringFormat(const char * format, ...)
{
	va_list args;
	va_start(args, format);
	std::string output = vStringFormat(format, args);
	va_end(args);
	return output;
}

const std::string EQEmuLogSys::vStringFormat(const char * format, va_list args)
{
	std::string output;
	va_list tmpargs;

	va_copy(tmpargs, args);
	int characters_used = vsnprintf(nullptr, 0, format, tmpargs);
	va_end(tmpargs);

	// Looks like we have a valid format string.
	if (characters_used > 0) {
		output.resize(characters_used + 1);

		va_copy(tmpargs, args);
		characters_used = vsnprintf(&output[0], output.capacity(), format, tmpargs);
		va_end(tmpargs);

		output.resize(characters_used);

		// We shouldn't have a format error by this point, but I can't imagine what error we
		// could have by this point. Still, return empty string;
		if (characters_used < 0)
			output.clear();
	}
	return output;
}

void EQEmuLogSys::ProcessConsoleMessage(uint16 debug_level, uint16 log_category, const std::string &message)
{
	uint16 color= GetWindowsConsoleColorFromCategory(log_category);

	int nLength = (int)logbox->SendMessage(WM_GETTEXTLENGTH);
	logbox->SetSel(nLength, nLength);
	logbox->ReplaceSel(message.c_str());
}

void EQEmuLogSys::Out(Logs::DebugLevel debug_level, uint16 log_category, std::string message, ...)
{

	bool log_to_dalog = true;
	if (log_settings[log_category].log_to_console < debug_level) {
		log_to_dalog = false;
	}

	bool log_to_file = true;
	if (log_settings[log_category].log_to_file < debug_level) {
		log_to_file = false;
	}

	bool log_to_gmsay = true;
	if (log_settings[log_category].log_to_gmsay < debug_level) {
		log_to_gmsay = false;
	}

	const bool nothing_to_log = !log_to_dalog && !log_to_file && !log_to_gmsay;
	if (nothing_to_log)
		return;
	message += "\n";
	va_list args;
	va_start(args, message);
	std::string output_message = vStringFormat(message.c_str(), args);
	va_end(args);

	std::string output_debug_message = EQEmuLogSys::FormatOutMessageString(debug_level,log_category, output_message);

	if (log_to_dalog) EQEmuLogSys::ProcessConsoleMessage(debug_level, log_category, output_debug_message);
	//if (log_to_gmsay) EQEmuLogSys::ProcessGMSay(debug_level, log_category, output_debug_message);
	if (log_to_file) EQEmuLogSys::ProcessLogWrite(debug_level, log_category, output_debug_message);
}

void EQEmuLogSys::SetCurrentTimeStamp(char* time_stamp)
{
	__time64_t raw_time;
	struct tm * time_info=new(tm);
	_time64(&raw_time);
	_localtime64_s(time_info,&raw_time);
	strftime(time_stamp, 80, "[%m-%d-%Y :: %H:%M:%S]", time_info);
}

void EQEmuLogSys::MakeDirectory(const std::string &directory_name)
{
#ifdef _WINDOWS
	struct _stat st;
	if (_stat(directory_name.c_str(), &st) == 0) // exists
		return;
	_mkdir(directory_name.c_str());
#else
	struct stat st;
	if (stat(directory_name.c_str(), &st) == 0) // exists
		return;
	mkdir(directory_name.c_str(), 0755);
#endif
}

void EQEmuLogSys::CloseFileLogs()
{
	if (process_log.is_open()) {
		process_log.close();
	}
}

void EQEmuLogSys::StartFileLogs(const std::string &log_name)
{
	EQEmuLogSys::CloseFileLogs();

	/* When loading settings, we must have been given a reason in category based logging to output to a file in order to even create or open one... */
	if (file_logs_enabled == false)
		return;

	if (EQEmuLogSys::log_platform) {
		if (!log_name.empty())
			platform_file_name = log_name;

		if (platform_file_name.empty())
			return;

		EQEmuLogSys::Out(Logs::General, Logs::Normal, "Starting File Log 'logs/%s_%i.log'", platform_file_name.c_str(), _getpid());
		EQEmuLogSys::MakeDirectory("logs/");
		process_log.open(StringFormat("logs/%s_%i.log", platform_file_name.c_str(), _getpid()), std::ios_base::app | std::ios_base::out);
	} else {
		if (platform_file_name.empty())
			return;

		EQEmuLogSys::Out(Logs::General, Logs::Normal, "Starting File Log 'logs/%s_%i.log'", platform_file_name.c_str(), _getpid());
		process_log.open(StringFormat("logs/%s_%i.log", platform_file_name.c_str(), _getpid()), std::ios_base::app | std::ios_base::out);
	}
}

BOOL EQEmuLogSys::setmainDlg(CDialog *mdlg)
{
	maindlg = mdlg;
	if(maindlg)
	return TRUE;
	return FALSE;
}
