#ifndef __AD_SETTINGS_H_
#define __AD_SETTINGS_H_
#include "settings.h"

#define SETTINGS_SECTION_DATA_NAME            "Data"
#define SETTINGS_SECTION_META_DATA_NAME       "MetaData"
#define SETTINGS_SECTION_META_DATA_VERSION    "Version"
#define SETTINGS_SECTION_META_DATA_SEQ_COUNT  "SequenceCount"
#define SETTINGS_SECTION_META_DATA_SIZE       "Size" //reserved(currently un-used)
#define SETTINGS_SECTION_META_DATA_CHECKSUM   "Checksum"
#define SHA1_FILE_EXTENSION                   "sha1"
#define ALTERNATE_FILE_EXTENSION              "alt"

#include "ADGenericChain.h"
/*
[Data]
sdis_selected_input = dvi1
sdis_scan_mode = auto

[MetaData]
Version       = 7
SequenceCount = 1234
Size     = 00000456
Checksum = 30216a22a37b5a825ea9430f05129332e98cfd4f #160bit sha-1 secure hash
*/
#define SETTINGS_STATUS_STRING_TABLE    {"OkMain","OkBkup","OkFmwOvride","NoFile","EmptyFileErr","EmptyFileDef","MainCorrupt","BkupCorrupt","BothCorrupt","unknown","none","\0"}
typedef enum ESETTINGS_STATE_T
{
	ESETTINGS_STATE_OK_MAIN, //everything is OK and main file is selected
	ESETTINGS_STATE_OK_BACKUP, //everything is OK and backup file is selected
	ESETTINGS_STATE_OK_OVERRIDE_DATA, //everything is OK, but due to new fmw, some settings have been overriden
	ESETTINGS_STATE_NO_FILE_PASSED,   //while starting the service, no settings file was specified
	ESETTINGS_STATE_EMPTY_FILE_CREATE_ERROR, //settings file was specified, but it is empty
	ESETTINGS_STATE_EMPTY_FILE_DEFAULT_DATA, //settings file was specified, but it is empty
	ESETTINGS_STATE_MAIN_FILE_CORRUPT,
	ESETTINGS_STATE_BACKUP_FILE_CORRUPT,
	ESETTINGS_STATE_BOTH_FILES_CORRUPT,
	ESETTINGS_STATE_UNKNOWN,
	ESETTINGS_STATE_NONE
}ESETTINGS_STATE;

#define SETTINGS_NAME_MAX_LENGTH 1024
typedef enum ESETTINGS_KEY_TYPE_T
{
	ESETTINGS_KEY_TYPE_STRING,
	ESETTINGS_KEY_TYPE_INT,
	ESETTINGS_KEY_TYPE_INT_ARR,
	ESETTINGS_KEY_TYPE_LONG,
	ESETTINGS_KEY_TYPE_LONG_ARR,
	ESETTINGS_KEY_TYPE_DOUBLE,
	ESETTINGS_KEY_TYPE_DOUBLE_ARR,
	ESETTINGS_KEY_TYPE_UNKNOWN,
	ESETTINGS_KEY_TYPE_NONE
}ESETTINGS_KEY_TYPE;

typedef enum ESETTINGS_KEY_STATE_T
{
	ESETTINGS_KEY_STATE_NOT_LOADED,
	ESETTINGS_KEY_STATE_LOADED,
	ESETTINGS_KEY_STATE_NOT_FOUND,
	ESETTINGS_KEY_STATE_STORED,
	ESETTINGS_KEY_STATE_UNKNOWN,
	ESETTINGS_KEY_STATE_NONE
}ESETTINGS_KEY_STATE;
typedef enum ESETTINGS_SELECTED_FILE_T
{
	ESETTINGS_SELECTED_FILE_MAIN,
	ESETTINGS_SELECTED_FILE_BACKUP,
	ESETTINGS_SELECTED_FILE_UNKNOWN,
	ESETTINGS_SELECTED_FILE_NONE
}ESETTINGS_SELECTED_FILE;

typedef enum ESETTINGS_SECTION_T
{
	ESETTINGS_SECTION_DATA,
	ESETTINGS_SECTION_META_DATA,
	ESETTINGS_SECTION_UNKNOWN,
	ESETTINGS_SECTION_NONE
}ESETTINGS_SECTION;


typedef struct SETTINGS_ENTRY_T
{
	unsigned int key_index;
	ESETTINGS_KEY_TYPE key_type;
	//char group[SETTINGS_NAME_MAX_LENGTH]; //group name
	char key[SETTINGS_NAME_MAX_LENGTH];   //key name
	void* value;//keep it as void because data type could be string/int/int_arr/long/double etc
	size_t value_size;//in case of array
	ESETTINGS_KEY_STATE key_state;
	ESETTINGS_SECTION section;
	char section_name[SETTINGS_NAME_MAX_LENGTH];//key name

}SETTINGS_ENTRY;

//following enum will be used as an ident for searching the entries in chain.
//0 to 64999 idents are served for user(caller service)
//this lib will use 65000 to 65006
typedef enum SETTINGS_META_ENTRY_T
{
	SETTINGS_META_ENTRY_VERSION=65000,
	SETTINGS_META_ENTRY_SEQCOUNT=65001,
	SETTINGS_META_ENTRY_SIZE=65002,
	SETTINGS_META_ENTRY_SHA1HASH=65003,
	SETTINGS_META_ENTRY_END=65004,
	SETTINGS_META_ENTRY_UNKNOWN=65005,
	SETTINGS_META_ENTRY_NONE=65006
}SETTINGS_META_ENTRY;

typedef struct SETTINGS_META_DATA_T
{
	int version      ;//meta_entry
	int seq_count    ;//meta_entry
	int size         ;//meta_entry //reserved(currently un-used)
	char sha1hash[50];//meta_entry //160bit+null_terminator+9extrabytes

	char file[1024];
	char alternate_file[1024];//used for next save
	ESETTINGS_SELECTED_FILE selected_file;
}SETTINGS_META_DATA;


class ADSettings: public ADChainConsumer
{
	SETTINGS_META_DATA MetaData;
	SETTINGS_ENTRY MetaStngTbl[5];//4meta entries + 1 extra_terminator
	ESETTINGS_STATE SettingsState;

	//ESETTINGS_SELECTED_FILE SelectedFile;
	int SettingsChainLocalID;
	ADGenericChain SettingsChain,SettingsChainLocal;//linked-list for storing all the keys(settings)
	char filepath[1024];
	Settings *settings;
	bool settings_open_flag;//tells if settings file is open or not

	//Chain-callback functions	
	virtual int identify_chain_element(void* element,int ident,ADChainProducer* pObj);
	virtual int double_identify_chain_element(void* element,int ident1,int ident2,ADChainProducer* pObj);
	virtual int free_chain_element_data(void* element,ADChainProducer* pObj);

	int Open();//char* file);
	int Save();//char* file);
	//int FileConsistencyCheck(char* file);
	int FileConsistencyCheck(char* mainfile,SETTINGS_META_DATA *pMetaData);
	int InitAndInsertMetaDataEntries();//SETTINGS_META_DATA *pMetaData);
	int ValidateMetaDataEntries();
	int UpdateSequenceCount();
	int CalculateCheckSum(char* file);
	bool VerifyCheckSum(char* file);
	//bool VerifySha1Checksum(char* file,char* sha1sum,int sumlen);
	int ReadMetaData(char* file,SETTINGS_META_DATA *pData);
	int SwapAlternateFile(SETTINGS_META_DATA *pData);

public:
	ADSettings();//char* file);
	~ADSettings();
	int Load(char* file);
	int Store();//char* file);//store to last loaded file
	int get_version();//char* version);
	int set_version(int version);//char* version);
	int get_seq_count(int* count);
	int get_size(unsigned int *size);
	int get_checksum(unsigned int *cs);
	int attach_setting(SETTINGS_ENTRY* pEntry);
	int init_setting(SETTINGS_ENTRY* pEntry,unsigned int indx,ESETTINGS_KEY_TYPE type,const char* name, void* val,size_t val_size,ESETTINGS_SECTION section=ESETTINGS_SECTION_DATA);
	int Is_it_valid_file(char* filepath);
	int ReadStatus(char *StsMsg);

};
#endif
