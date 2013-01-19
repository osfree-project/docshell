/**************************************************************************
 *
 *  Copyright 2008, Roger Brown
 *
 *  This file is part of Roger Brown's Toolkit.
 *
 *  This program is free software: you can redistribute it and/or modify it
 *  under the terms of the GNU Lesser General Public License as published by the
 *  Free Software Foundation, either version 3 of the License, or (at your
 *  option) any later version.
 * 
 *  This program is distributed in the hope that it will be useful, but WITHOUT
 *  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 *  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 *  more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>
 *
 */

/*
 * $Id$
 */

#ifndef _ODRES_
#define _ODRES_

#ifndef DID_OK
#define DID_OK                     0x0001
#endif
#ifndef DID_CANCEL
#define DID_CANCEL                 0x0002
#endif
#ifndef DID_HELP
#define DID_HELP                   0x0004
#endif
#ifndef DID_ERROR
#define DID_ERROR                  0xFFFF
#endif

#define ODHELPFILE "opendoc.hlp"

#define ID_ICON                     101    
#define ID_BORDER_PTR               102   

#define OPENDOC_BITMAP              201
#define OPENDOC_BITMAP2             202s

#define IDD_TYPEPAGEDLG            1000
#define DID_CATEGORYTEXT           1001
#define DID_CATEGORY               1002
#define DID_KINDTEXT               1003
#define DID_KIND                   1004
#define DID_EDITORTEXT             1005
#define DID_EDITOR                 1006
#define DID_UNDO                   1008
#define DID_DEFAULT                1009

#define IDD_VIEWPAGEDLG            1020
#define DID_SHOWASTEXT             1021
#define DID_VIEW                   1022
#define DID_BUNDLED                1023
#ifndef _PLATFORM_OS2_
#define DID_NAME                   1024
#endif
#define DID_STATIONARY             1025
#define DID_SHOWLINKS              1026
#define DID_PARTNAME               1027

#define IDD_PRESENTATIONPAGEDLG    1040
#define DID_PRESENTASTEXT          1041
#define DID_PRESENTATION           1042

#define ID_ODINFO                  1044
#define IDC_NOTEBOOK               1045
#ifdef _WIN32
	#define WM_SET_ACTIVE              (WM_USER + 0)
	#define WM_DID_OK                  (WM_USER + 1)
#endif
#define IDD_NOTEBOOKDLG            1046
#define IDC_OK                     1047
#define IDC_CANCEL                 1048
#define IDC_APPLY                  1049
#define DID_VIEWASTEXT             1050
#define GROUP1                     1051
#define DID_DUMMY2                 1052
#define DID_DUMMY3                 1053
#define DID_DUMMY4                 1054
#define DID_DUMMY5                 1055
#define DID_DUMMY6                 1057
#define DID_DUMMY7                 1058
#define IDD_SETTINGS               1059

#define IDD_GENERALPAGEDLG         1060
#define DID_OBJECTTITLE            1061
#define DID_ICONCURRENT            1062
#define DID_ICONCRANOTHER          1063
#define DID_ICONEDIT               1064
#define DID_ICONFIND               1065
#define DID_RESET                  1067
#define DID_ICONHELP               1068

#define IDD_ICONVIEWPAGEDLG        1080
#define DID_DUMMY                  1081
#define DID_FLOW                   1082
#define DID_NOFLOW                 1083
#define DID_NOGRID                 1084
#define DID_ICONNORM               1085
#define DID_ICONSMALL              1086
#define DID_ICONINV                1087
#define DID_ICONVISIBLETEXT        1088
#define DID_ICONCHFONT             1089

#define IDD_TREEVIEWPAGEDLG        1100
#define DID_LINES                  1101
#define DID_NOLINES                1102
#define DID_TRNORM                 1103
#define DID_TRSMALL                1104
#define DID_TRINV                  1105
#define DID_TRVISIBLETEXT          1107
#define DID_TRCHFONT               1108

#define IDD_DETAILSVIEWPAGEDLG     1120
#define DID_ICONCHBGCOLOR          1121
#define DID_OBJECTTYPE             1122
#define DID_DETAILS                1123
#define DID_CHANGEFONT             1124

#define IDD_FILEPAGE1DLG           1140
#define DID_CRDATE                 1141
#define DID_MODDATE                1142
#define DID_ACCDATE                1143
#define DID_MODBY                  1144
#define DID_FILEPAGE1HELP          1145
#define DID_REMOVE                 1146
#define DID_DATASIZE               1147

#define IDD_FILEPAGE2DLG           1160
#define DID_COMMENTS               1161
#define DID_KEY                    1162
#define DID_HISTORY                1163
#define DID_FILEPAGE2HELP          1164
#define DID_SETTINGS               1165

#define IDD_LSPAGEDLG              1180
#define IDD_LSKINDTXT              1181
#define IDD_LSCREATEDTXT           1182
#define IDD_LSUPDATEDTXT           1183
#define IDD_LSONSAVEBTN            1184
#define DID_LSMANUALLYBTN          1185
#define DID_LSBREAKLINKBTN         1186
#define DID_LSUPDATENOWBTN         1187

#define IDD_EDITLSDESCDLG          1201
#define IDD_EDITLSDESC             1202

#define IDD_EDITLTDESCDLG          1220
#define IDD_EDITLTDESC             1221

#define IDD_LDPAGEDLG              1240
#define IDD_LDKINDTXT              1241
#define IDD_LDCREATEDTXT           1242
#define IDD_LDUPDATEDTXT           1243
#define IDD_LDAUTOMATICALLYBTN     1244
#define DID_LDMANUALLYBTN          1245
#define DID_LDBREAKLINKBTN         1246
#define DID_LDUPDATENOWBTN         1247
#define IDD_LDFINDSOURCEBTN        1248

#define IDD_LUDLG                  1250
#define IDD_LUUPDATE               1251
#define IDD_LUSTOP                 1252

#define IDD_PASTEASDLG             1260
#define IDD_PASTELINKCHKBOX        1261
#define IDD_PASTEAUTOBTN           1262
#define IDD_PASTEMANUALBTN         1263
#define IDD_GETUPDATESTXT          1264
#define IDD_MERGEBTN               1266
#define IDD_EMBEDBTN               1267
#define IDD_EMBEDAS                1268
#define IDD_PARTKIND               1269
#define IDD_PARTEDITOR             1270
#define DID_PASTE                  1271
#define DID_PASTEASHELP            1272

#define IDD_MOVECOPYDLG            1280
#define IDD_OLDFILENAME            1281
#define IDD_NEWFILENAME            1282
#define DID_FILECOPIED             1283
#define DID_FILEMOVED              1284

#define IDD_COPYLINKTODLG          1301
#define ID_LINKDESC_TXT            1302
#define ID_LINKDESC_ED             1303
#ifndef _PLATFORM_OS2_
#define DID_FILENAME_TXT           1304
#define DID_FILENAME_ED            1305
#define DID_DRIVE_TXT              1306
#define DID_DRIVE_CB               1307
#define DID_FILES_TXT              1308
#define DID_FILES_LB               1309
#define DID_DIRECTORY_TXT          1310
#define DID_DIRECTORY_LB           1311
#define DID_FILTER_TXT             1312
#define DID_FILTER_CB              1313
#endif

#define IDD_PASTELINKFROMDLG       1320
#define ID_LINKDESC_LB             1321

#define IDD_PRODUCTINFO            1340
#define IDR_PRODUCTINFO            1341

#define IDD_SELPARTKIND            1360
#define DID_KINDMSG                1361
#define DID_KINDLIST               1362
#define DID_OPENFILE               1363

#define IDD_DRAFTHISTORYDLG        1380
#define DID_DRAFTLBOX              1381
#define DID_OPEN                   1382
#define DID_DELETE                 1383
#define DID_DONE                   1384
#define DID_CREATOR                1385
#define DID_DRAFT                  1386
#define DID_CREATED                1387
#define DID_CMNT                   1388
#define DID_DRAFTCNTR              1389
#define DID_DRAFTDATELABEL         1390

#define IDD_DELETEDRAFTDLG         1400
#define DID_DRAFTDELETEMSG         1401

#define IDD_CREATEDRAFTDLG         1420
#define DID_DRAFTNAMELABEL         1421
#define DID_DRAFTNAME              1422
#define DID_DRAFTNUMLABEL          1423
#define DID_DRAFTNUM               1424
#define DID_DRAFTCMNTSLABEL        1425
#define DID_DRAFTCOMMENT           1426
#define DID_SAVE                   1427
#define DID_DISCARD                1428

#define ID_BASEMENUBAR             500    

#define  IDMS_DOCUMENT             2000    
#define  DOC_NEW                   2001
#define  DOC_OPENAS                2002
#define  DOC_INSERT                2003
#define  DOC_SEPARATOR1            2004
#define  DOC_SAVE                  2005
#define  DOC_SAVECOPY              2006
#define  DOC_REVERTTOSAVED         2007
#define  DOC_SEPARATOR2            2008
#define  DOC_DRAFTS                2009    
#define  DOC_DRAFTCREATE           2010
#define  DOC_DRAFTHISTORY          2011
#define  DOC_PROPERTIES            2012
#define  DOC_SETUP                 2013
#define  DOC_SEPARATOR3            2014
#define  DOC_PRINT                 2015
#define  DOC_SEPARATOR4            2016
#define  DOC_CLOSE                 2017

#define  IDMS_EDIT                 2100    
#define  EDIT_UNDO                 2101
#define  EDIT_REDO                 2102
#define  EDIT_SEPARATOR1           2103
#define  EDIT_CREATE               2104
#define  EDIT_CUT                  2105
#define  EDIT_COPY                 2106
#define  EDIT_PASTE                2107
#define  EDIT_PASTEAS              2108
#define  EDIT_LINK_MENU            2109    
#define  EDIT_PASTELINK            2110
#define  EDIT_BREAKLINK            2111
#define  EDIT_COPYLINKTO           2112
#define  EDIT_PASTELINKFROM        2113
#define  EDIT_SEPARATOR2           2114
#define  EDIT_DELETE               2115
#define  EDIT_SEPARATOR3           2116
#define  EDIT_SELECTALL            2117
#define  EDIT_DESELECTALL          2118
#define  EDIT_SEPARATOR4           2119
#define  EDIT_OPENSEL              2120
#define  EDIT_SELPROPERTIES        2121
#define  EDIT_INSERT               2122
#define  EDIT_SEPARATOR5           2124
#define  EDIT_VIEWINWINDOW         2312  
#define  EDIT_LINKINFO             2125

#define  IDMS_HELP                 2200   
#define  HELP_INDEX_OD             2201
#define  HELP_GENERAL              2202
#define  HELP_USING                2203
#define  HELP_SEPARATOR1           2204
#define  HELP_PRODUCTINFO          2205

#define  IDMS_VIEW                 2350   
#define  VIEW_OPENAS               2300   
#define  VIEW_OAICON               2301
#define  VIEW_OATREE               2302
#define  VIEW_OADETAILS            2303
#define  VIEW_OAWINDOW             2304
#define  VIEW_PROPERTIES           2305
#define  VIEW_SHOWAS               2306   
#define  VIEW_SASMALLICON          2307
#define  VIEW_SALARGEICON          2308
#define  VIEW_SATHUMBNAIL          2309
#define  VIEW_SAFRAME              2310
#define  VIEW_SEPARATOR1           2311
#define  VIEW_VIEWINWINDOW         2312

#define  ID_BASEPOPUP               501   
#define  ID_HELPSUBMENU             502   
#define  ID_SHOWASSUBMENU           503   
#define  ID_OPENASSUBMENU           504   
#define  ID_ACCELTABLE              505   
#define  ID_CONTAINERVIEWPOPUP      506   
#define  ID_CONTAINERVIEWMENUBAR    507   

#define IDS_DOCUMENT               4001
#define IDS_DOCNEW                 4002
#define IDS_DOCOPENAS              4003
#define IDS_INSERT                 4004
#define IDS_DOCSAVE                4005
#define IDS_SAVECOPY               4006
#define IDS_REVERTTOSAVED          4007
#define IDS_DRAFTS                 4008
#define IDS_DRAFTCREATE            4009
#define IDS_DRAFTHISTORY           4010
#define IDS_DOCPROPERTIES          4011
#define IDS_SETUP                  4012
#define IDS_DOCPRINT               4013
#define IDS_CLOSE                  4014
#define IDS_EDIT                   4015
#define IDS_UNDO                   4016
#define IDS_REDO                   4017
#define IDS_CREATE                 4018
#define IDS_CUT                    4019
#define IDS_COPY                   4020
#define IDS_PASTE                  4021
#define IDS_PASTEAS                4022
#define IDS_LINK                   4023
#define IDS_PASTELINK              4024
#define IDS_BREAKLINK              4025
#define IDS_COPYLINKTO             4026
#define IDS_PASTELINKFROM          4027
#define IDS_DELETE                 4028
#define IDS_SELECTALL              4029
#define IDS_DESELECTALL            4030
#define IDS_OPENSEL                4031
#define IDS_SELPROPERTIES          4032
#define IDS_HELP                   4033
#define IDS_HELPINDEX              4034
#define IDS_HELPGENERAL            4035
#define IDS_HELPUSING              4036
#define IDS_PRODUCTINFO            4037
#define IDS_HIDELINKS              4038
#define IDS_FRAME                  4039
#define IDS_LARGEICON              4040
#define IDS_SMALLICON              4041
#define IDS_THUMBNAIL              4042
#define IDS_BINDINGWARNING         4043
#define IDS_BINDINGWARNING_TITLE   4044
#define IDS_NOPARTWARNING          4045
#define IDS_VIEWINWINDOW           4046
#define IDS_NOSERVER               4047
#define IDS_ERROR                  4048

#ifdef DEBUG
#define  IDMS_BENTO_FILE           5000
#define  IDMS_BENTO_CREATE         5001
#define  IDMS_BENTO_OPEN           5002
#define  IDMS_BENTO_SAVE           5003
#define  IDMS_BENTO_TEXT           5004
#define  IDMS_BENTO_ROOT           5005
#define  IDMS_BENTO_CLIP           5006
#define  ID_BENTO                  5007
#endif

#define CV_DETAIL_VIEW             6000
#define CV_ICON_VIEW               6001
#define CV_TREE_VIEW               6002
#define CV_ITALIC                  6003
#define CV_UNDERSCORE              6004
#define CV_STRIKEOUT               6005
#define CV_BOLD                    6006
#define CV_OUTLINE                 6007
#define CV_ICON_TEXT               6008
#define CV_TITLE                   6009
#define CV_CATEGORY                6010
#define CV_KIND                    6011
#define CV_CDATE                   6012
#define CV_CTIME                   6013
#define CV_LST_MOD_DATE            6014
#define CV_LST_MOD_TIME            6015
#define CV_LST_MOD_USER            6016
#define CV_SIZE                    6017
#define CV_UNTITLED                6018
#define CV_UNKNOWN                 6019
#define CV_SYSTEM_INI              6020
#define CV_PARTS                   6021
#define CV_POSITION                6022
#define CV_FONTS                   6023
#define CV_BCK_COLOR               6024
#define CV_ICON_TXT_CLR            6025
#define CV_OD_ICONVIEW             6026
#define CV_OD_TREEVIEW             6027
#define CV_OD_DETAILVIEW           6028
#define SCV_WPSDLGFONT             6029  
#define SCV_DLGPREVIEWTEXT         6030
#define SCV_VIEWCHGFONT            6031
#define SCV_DETVIEWCHGFONT         6032
#define SCV_TREEVIEWCHGFONT        6034
#define SCV_OD_SETTINGSNAME        6033
#define SCV_ICONEDIT_EXE           6035
#define SCV_STATUS_TEXT_TEMPLATE   6036
#define SCV_TEMP_FILE_APPNAME      6037
#define SCV_TYPE                   6038
#define SCV_VIEW                   6039
#define SCV_FILE                   6040
#define SCV_GENERAL                6041
#define SCV_TITLE_NB_FRAME         6042
#define SCV_TAB_TXT_FONT           6043
#define SCV_LINK_SOURCE            6044
#define SCV_LINK_TARGET            6045
#define SCV_LINK_TIME              6046
#define SCV_LT_DESC                6047
#define SCV_LT_CONNDATA            6048
#define SCV_LS_DESC                6049
#define SCV_LS_CONNDATA            6050
#define SCV_LINK_NAME              6051
#define SCV_LINK_TGT_NAME          6052

#define ID_DRAFT_TITLE             7001
#define ID_DRAFT_WINTITLE          7002
#define ID_DRAFT_RUSURE1           7003
#define ID_DRAFT_RUSURE2           7004
#define ID_DRAFT_NUMBER            7005
#define ID_DRAFT_DELETE            7006
#define ID_DRAFT_INFO              7007
#define ID_DRAFT_COLICON           7008
#define ID_DRAFT_COLTITLE          7009
#define ID_DRAFT_COLDRAFT          7010
#define ID_DRAFT_COLCREATOR        7011
#define ID_DRAFT_COLCRDATE         7012
#define ID_DRAFT_COLCRTIME         7013
#define ID_DRAFT_COLCOMMENTS       7014
#define ID_DRAFT_ICONVW            7015
#define ID_DRAFT_DETAILVW          7016
#define ID_DRAFTWND_PROPS          7017
#define ID_DRAFT_ICON              7018
#define ID_DRAFT_DETAILS           7019
#define ID_DRAFT_DETAILSVIEWPAGEDLG 7020
#define ID_DRAFT_ICONVIEWPAGEDLG   7021
#define ID_DRAFT_WARN_TITLE        7022
#define ID_DRAFT_NUM_LABEL         7023
#define ID_DRAFT_NAME_LABEL        7024
#define ID_DRAFT_COMMENTS_LABEL    7025
#define ID_DRAFT_OPEN_FAIL         7026
#define ID_DRAFT_DELETE_FAIL       7027
#define ID_DRAFT_DELETE_MSG        7028
#define ID_DRAFT_HELP_MSG          7029
#define ID_DRAFT_CREATE            7030

#define OD_PROCESSAETE             8000
#define OD_PROCESSSUITE            8001
#define OD_PROCESSEVENT            8002
#define OD_PROCESSCLASS            8003
#define OD_PROCESSOPERATOR         8004
#define OD_PROCESSENUMERATION      8005
#define OD_PROCESSPARAMETER        8006
#define OD_PROCESSPROPERTY         8007
#define OD_PROCESSELEMENT          8008
#define OD_PROCESSENUMERATOR       8009
#define OD_PROCESSERROR            8011
#define OD_AEUTOBJECT              8012
#define OD_AEUTOVERRIDE            8013
#define OD_VERSIONS                8014
#define OD_SUITENUMBER             8015
#define OD_NAMEDESC                8016
#define OD_NAMEDESC1               8017
#define OD_NAMEDESC2               8018
#define OD_EVENTNUMBER             8019
#define OD_CLASSNUMBER             8020
#define OD_OPERATORNUMBER          8021
#define OD_ENUMNUMBER              8022
#define OD_PARAMETERNUMBER         8023
#define OD_PROPERTIESNUMBER        8024
#define OD_ELEMENTNUMBER           8025
#define OD_ENUMERATIONID           8026
#define OD_PARAMETERS              8027
#define OD_PROPERTY                8028
#define OD_ELEMENTCLASS            8029
#define OD_KEYFORMS                8030
#define OD_KEYFORMID               8031
#define OD_OPENDOC                 8032
#define OD_NONAMESPACE             8033
#define OD_NODOCPROPERTIES         8034
#define OD_READINGITEXT            8035
#define OD_VERSIONLIST             8036
#define OD_IMPORTLIST              8037
#define OD_EXPORTLIST              8038
#define OD_NOCREATECONTAINER       8039
#define OD_NOREADCONTAINER         8040
#define OD_INVALCONTAINER          8041
#define OD_NOLABELCONTAINER        8042
#define OD_NONEWVALUE              8043
#define OD_INCORRECTLEN            8044
#define OD_EMBEDDEDINSERTS         8045
#define OD_EMBEDDEDDELETES         8046
#define OD_NOCREATETARGET          8047
#define OD_NOREADTARGET            8048
#define OD_INVALIDTARGET           8049
#define OD_TARGETNOTREAD           8050
#define OD_SELFDYING               8051
#define OD_OPENDOCHELP             8052
#define OD_REALOPENDOCHELP         8053
#define OD_NODEFINITION            8054
#define OD_NOCANVASOWNER           8055
#define OD_OPENDOCEXCEPTION        8056
#define OD_INVALIDLINK             8057
#define OD_UNEXPECTEDSTATE         8058
#define OD_REACHEDDEFAULT          8059
#define OD_OPENDOCSHELL            8060
#define OD_OPENDOCPRINT            8061
#define OD_UNKNOWN                 8062
#define OD_THROWERROR              8063
#define OD_ENVERROR                8064
#define OD_UNKNOWNSOM              8065
#define OD_OUTOFMEMORY             8066
#define OD_EXCEPTIONS              8067
#define OD_ALLOCATEFAILEDCONTAINER 8068
#define OD_ALLOCATEFAILED          8069
#define OD_CANNOTCREATECONTAINER   8070
#define OD_CANNOTREADCONTAINER     8071
#define OD_INVALIDCONTAINER        8072
#define OD_CANNOTREADLABEL         8073
#define OD_CANNOTWRITELABEL        8074
#define OD_CANNOTOPEN              8075
#define OD_INCORRECTBYTELEN        8076
#define OD_ALLOCATIONERROR         8077
#define OD_CONTAINERHANDLE         8078
#define OD_PREVIOUSDOCUMENT        8079
#define OD_GOODDOCUMENT            8080
#define OD_UNTITLEDDOCUMENT        8081
#define OD_NOKINDS                 8082
#define OD_NOEDITORS               8083
#define OD_NOKINDS2                8084
#define OD_NOEDITORS2              8085
#define OD_NOUSERCATEGORY          8086
#define OD_NOPROMISE               8087

#define IDS_CLOSECAPTION        10000
#define IDS_CLOSEPROMPT         10001
#define IDS_EMBEDDINGCAPTION    10002
#define IDS_EXIT                10003
#define IDS_EXITANDRETURN       10004
#define IDS_UPDATE              10005
#define IDS_UNNAMED             10006
#define IDS_OLE_DOCUMENT        10007
#define IDS_OLE_EDIT            10008
#define IDS_OLE_HELP            10009
#define IDS_OLECONTAINER        10010

#define DOC_EXIT                10020
#define DOC_UPDATE              10021

#endif
