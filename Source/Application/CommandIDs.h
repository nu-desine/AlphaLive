

//  Created by Liam Meredith-Lacey on 01/05/2012.
//  Copyright 2012 nu desine.
//
//  This file is part of AlphaLive.
//
//  AlphaLive is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License, version 2, 
//  as published by the Free Software Foundation.
//  
//  AlphaLive is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.
//


#ifndef COMMANDIDS_H
#define COMMANDIDS_H

namespace CommandIDs
{
    static const int About					= 0x2FFFE;
	static const int Preferences            = 0x2FFFF;
    static const int New                    = 0x20014;
	static const int Open                   = 0x20001;
    static const int Save                   = 0x20002;
    static const int SaveAs                 = 0x20003;
    static const int ProjectSettings        = 0x20004;
    static const int CleanUpProject         = 0x20005;
    
    static const int SaveScene             = 0x20006;
    static const int LoadScene             = 0x20007;
    
    static const int ClearScene            = 0x20008;
    static const int ClearAllScenes        = 0x20009;
    
    static const int CopyPadSettings        = 0x20016; //use StandardApplicationCommandIDs here?
    static const int PastePadSettings       = 0x20017; //use StandardApplicationCommandIDs here?
    
    static const int DisableHelpBox         = 0x20010;
    
    static const int StartStopClock         = 0x20011;
    static const int KillSwitch             = 0x20012;
    
    static const int updateFirmware         = 0x20018;
    static const int UpdateSoftware         = 0x20021;
    
    static const int StarterGuide           = 0x20019;
    static const int ReferenceManual        = 0x20020;
    
    static const int CopyDataToSequencer    = 0x20022;
    
    static const int EnableLed              = 0x20030;
    static const int EnableLedPressure      = 0x20031;
    static const int EnableLedClock         = 0x20032;
    static const int HardwarePreferences    = 0x20033;
    static const int EnableLedMidiMode      = 0x20034;
    static const int HardwareProjectSettings = 0x20035;
    
    static const int SendMidiClock          = 0x20036;
    static const int SyncToMidiClock        = 0x20037;
    static const int MidiClockSettings      = 0x20038;
    
    static const int WebsiteHomeLink        = 0x20040;
    static const int WebsiteTutorialsLink   = 0x20041;
    static const int WebsiteSupportLink     = 0x20042;
    static const int WebsiteForumLink       = 0x20043;
    
    static const int ViewTriggerSettings    = 0x20050;
    static const int ViewPressureSettings   = 0x20051;
    static const int ViewGlobalPadSettings  = 0x20052;
    static const int ViewScenePresets       = 0x20053;
    static const int EnabledPadContentsDisplay = 0x20054;
    
}

namespace CommandCategories
{
    static const char* const OtherCommands			= "Other";
	static const char* const FileCommands			= "File";
    static const char* const ControlCommands		= "Control";
    static const char* const EditCommands           = "Edit";
    static const char* const OptionCommands         = "Option";
    static const char* const HardwareCommands       = "Hardware";
    static const char* const ViewCommands           = "View";
}

#endif // COMMANDIDS_H