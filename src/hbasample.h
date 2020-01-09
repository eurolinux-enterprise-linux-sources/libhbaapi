/*
 * Description
 *         hbasample.h - Header file for sample HBA API compliant vendor library
 *
 * License:
 *	The contents of this file are subject to the SNIA Public License
 *	Version 1.0 (the "License"); you may not use this file except in
 *	compliance with the License. You may obtain a copy of the License at
 *
 *	/http://www.snia.org/English/Resources/Code/OpenSource.html
 *
 *	Software distributed under the License is distributed on an "AS IS"
 *	basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See
 *	the License for the specific language governing rights and limitations
 *	under the License.
 *
 * The Original Code is  Header file for
 *	 SNIA HBA API Sample Vendor Specific Library
 *
 * The Initial Developer of the Original Code is:
 *	Benjamin F. Kuo, Troika Networks, Inc. (benk@troikanetworks.com)
 *
 * Contributor(s):
 *
 ***************************************************************************
 */
#ifdef WIN32
#ifdef HBASAMPLE_EXPORTS
#define HBASAMPLE_API __declspec(dllexport)
#else
#define HBASAMPLE_API __declspec(dllimport)
#endif
#else
#define HBASAMPLE_API
#endif


HBASAMPLE_API HBA_STATUS HBA_RegisterLibrary( PHBA_ENTRYPOINTS entrypoints );
