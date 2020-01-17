/*
 ******************************************************************************
 * unix2dos.cpp - Convert line breaks in a text file from UNIX-style to       *
 *                DOS-style.                                                 *
 *                                                                            *
 *   Copyright 2005, 2013 Michael Wright                                      *
 *                                                                            *
 *   This program is free software: you can redistribute it and/or modify     *
 *   it under the terms of the GNU General Public License as published by     *
 *   the Free Software Foundation, either version 3 of the License, or        *
 *   (at your option) any later version.                                      *
 *                                                                            *
 *   This program is distributed in the hope that it will be useful,          *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of           *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            *
 *   GNU General Public License for more details.                             *
 *                                                                            *
 *   You should have received a copy of the GNU General Public License        *
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.    *
 *                                                                            *
 * Revision History:                                                          *
 * Rev. 1.0 05-21-2005 Michael Wright - Initial release.                      *
 * Rev. 1.1 07-02-2013 Michael Wright - Bug fix for problem on Windows.       *
 * Rev. 1.2 07-06-2013 Michael Wright - Minor changes, and license file to    *
 *                                      release under terms of GNU GPL.       *
 *                                                                            *
 ******************************************************************************
 *
 */

#include <iostream>
#include <fstream>
#include <cstdio>   // file functions
#include <unistd.h>  // getopt()
using namespace std;

extern char *optarg;  // getopt() - will point to option arg eg. -s foobar
extern int optind, opterr; // getopt()

void ShowUsage();
void ShowProgramInfo();

string programVersion = string( "V1.2" );
const char CR = '\015';
const char NL = '\012';

int main(int argc, char *argv[])
{

    string inFilename;
    string tmpOutFilename;
    string tmpBackupname;

    int QuietMode, DebugMode;

    // Set defaults
    QuietMode = 0;
    DebugMode = 0;

    int c;
    opterr = 1;  // non-zero enables getopt() error message
    while ((c=getopt(argc, argv, "qdhv")) != -1) {
        switch(c)
        {
            case 'q':
                QuietMode = 1;
                break;
            case 'd':
                DebugMode = 1;
                break;
            case 'h':
                ShowUsage();
                return 0;
            case 'v':
                ShowProgramInfo();
                return 0;
            case '?':
                cout << "   Unexpected option" << endl;
                ShowUsage();
                return -1;
        }
    }

    if ( optind >= argc ) {
        cout << "No files to convert!" << endl;
        ShowUsage();
        return -1;
    }

    if ( ! QuietMode )
        cout << "UNIX to DOS file conversion" << endl;

    for (int i=optind; i<argc; i++) {
        inFilename = string(argv[i]);
        if ( ! QuietMode )
            cout << "   Processing : " << inFilename << endl;
        // For temp filenames, remove any preceding path to force file to
        // current dir
        tmpOutFilename = string( tempnam(".", "u2d") );
        tmpOutFilename.erase(0, (tmpOutFilename.find_last_of("\\/")+1) );
        if ( DebugMode )
            cout << "      DEBUG: tmpOutFilename = " << tmpOutFilename << endl;
        tmpBackupname = string( tempnam(".", "bak") );
        tmpBackupname.erase(0, (tmpBackupname.find_last_of("\\/")+1) );
        if ( DebugMode )
            cout << "      DEBUG: tmpBackupname = " << tmpBackupname << endl;

        ifstream inFile( inFilename.c_str(), ios::binary  );
        if (! inFile) {
            cout << "Error: Can't open input file: " << inFilename << endl;
            return -1;
        }
        ofstream outFile( tmpOutFilename.c_str(), ios::binary  );
        if (! outFile) {
            cout << "Error: Can't open output file: " << tmpOutFilename << endl;
            return -1;
        }

        char ch, previous_ch;
        previous_ch = NULL;
        while( inFile.get(ch) ) {
            if (ch == NL && previous_ch != CR)
               outFile.put( CR );
            outFile.put(ch);
            previous_ch = ch;
        }
        inFile.close();
        outFile.close();

        if ( rename( inFilename.c_str(), tmpBackupname.c_str()) ) {
            cout << "Error renaming original input file (" << inFilename <<")" << endl;
            remove( tmpOutFilename.c_str() );
            return -1;
        }
        if ( rename( tmpOutFilename.c_str(), inFilename.c_str()) ) {
            cout << "Error renaming result file (" << tmpOutFilename << ")" << endl;
            // restore originale file
            rename( tmpBackupname.c_str(), inFilename.c_str());
            remove( tmpOutFilename.c_str() );
            return -1;
        }
        // delete original input file (renamed)
        remove( tmpBackupname.c_str() );
    }
    if ( ! QuietMode )
        cout << "Done." << endl;
    return 0;
}

void ShowUsage() {
    cout << "UNIX to DOS file conversion.\n" << endl;
    cout << "Usage: unix2dos {-q -d} filename filename ..." << endl;
    cout << "       unix2dos {-h -v}" << endl;
    cout << "       -q : Quiet mode" << endl;
    cout << "       -d : Debug mode" << endl;
    cout << "       -h : Show program usage" << endl;
    cout << "       -v : Show program version\n" << endl;
   return;
}
void ShowProgramInfo() {
    cout << "unix2dos " << programVersion << "  Copyright (C) 2005, 2013   Michael Wright" << endl;
    cout << "This program comes with ABSOLUTELY NO WARRANTY." <<endl;
    cout << "This is free software, and you are welcome to redistribute it under certain" <<endl;
    cout << "conditions; see terms of GNU GPL (http://www.gnu.org/licenses/)\n" <<endl;
   return;
}
