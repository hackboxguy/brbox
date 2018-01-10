#include "ADSettings.hpp"
#include "settings.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "ADCommon.hpp"

//#include <iostream>
//#include <stdlib.h>
//#include <sys/mman.h>
//#include <fcntl.h>
//#include <unistd.h>
/*****************************************************************************/
using namespace std;
/*****************************************************************************/
//chain-callback functions
int ADSettings::identify_chain_element(void* element,int ident,ADChainProducer* pObj)
{
	SETTINGS_ENTRY *pEntry;
	pEntry=(SETTINGS_ENTRY*)element;
	if((int)pEntry->key_index==ident)
		return 0;
	return -1;
}
int ADSettings::double_identify_chain_element(void* element,int ident1,int ident2,ADChainProducer* pObj)
{
	return -1;
}
void ADSettings::delete_value(ESETTINGS_KEY_TYPE key_type,void *value)
{
	switch(key_type)
	{
		case ESETTINGS_KEY_TYPE_STRING    :
			{char *myptr=(char*)value;ARRAY_MEM_DELETE(myptr);}
			break;
		case ESETTINGS_KEY_TYPE_INT       :
			{int *myptr=(int*)value;ARRAY_MEM_DELETE(myptr);}
		case ESETTINGS_KEY_TYPE_INT_ARR   :
			{int *myptr=(int*)value;ARRAY_MEM_DELETE(myptr);}
		case ESETTINGS_KEY_TYPE_LONG      :
			{long *myptr=(long*)value;ARRAY_MEM_DELETE(myptr);}
		case ESETTINGS_KEY_TYPE_LONG_ARR  :
			{long *myptr=(long*)value;ARRAY_MEM_DELETE(myptr);}
		case ESETTINGS_KEY_TYPE_DOUBLE    :
			{double *myptr=(double*)value;ARRAY_MEM_DELETE(myptr);}
		case ESETTINGS_KEY_TYPE_DOUBLE_ARR:
			{double *myptr=(double*)value;ARRAY_MEM_DELETE(myptr);}
		default:break;
	}
}
int ADSettings::free_chain_element_data(void* element,ADChainProducer* pObj)
{
	if(pObj->getID() == SettingsChainLocalID)
	{
		SETTINGS_ENTRY *objData;
		objData=(SETTINGS_ENTRY *)element;
		if(objData->value!=NULL)
			//MEM_DELETE(objData->value);
			//ARRAY_MEM_DELETE(objData->value);
			delete_value(objData->key_type,objData->value);
	}
	return 0;
}
/*****************************************************************************/
ADSettings::ADSettings()//(char* file)
{
	SettingsState=ESETTINGS_STATE_UNKNOWN;
	filepath[0]='\0';
	//strcpy(filepath,file);
	settings = NULL;
	settings_open_flag=false;//settings is not open yet
	SettingsChain.attach_helper(this);
	SettingsChain.disable_auto_remove();

	//local chain for comparing changed settings
	SettingsChainLocalID=SettingsChainLocal.attach_helper(this);
}
/*****************************************************************************/
ADSettings::~ADSettings()
{
	//disconnect();//freeup the memory
	if(settings!=NULL)
		settings_delete(settings);
}
/*****************************************************************************/
int ADSettings::Is_it_valid_file(char* filepath)
{
	struct stat buffer;
	if(stat(filepath,&buffer)!=0)
		return -1;
	if(buffer.st_mode & S_IFREG)
		return 0;
	return -1;//it could be a directory
}
/*****************************************************************************/
int ADSettings::Open()//char* file)
{
	if(settings_open_flag)
	{
		printf("settings file is already open!\n");
		return 0;//settings is already open
	}
	if(Is_it_valid_file(filepath)!=0)
	{
		//printf("settings file :%s not found for reading!\n",filepath);
		return -1;
	}

	FILE *f;
//	printf("loading %s\n",filepath);
	f = fopen(filepath, "r");
	if (f == NULL)
	{
	    	/* Handle error... */
		//printf("file not found for reading!\n");
		return -1;
	}
	settings = settings_open(f);
	fclose(f);
	if (settings == NULL)
	{
	    	/* Handle read error... */
		//printf("unable to open settings\n");
		return -1;
	}
	settings_open_flag=true;
	return 0;
}
/*****************************************************************************/
int ADSettings::Save()
{
	if(settings_open_flag==false)
	{
		//printf("settings file is not open!\n");
		return -1;
	}
	/* Save the settings object to disk in textual form */
	FILE *f;
	//printf("saving %s\n",filepath);
	f = fopen(filepath, "w");
	if (f == NULL)
	{
	    	/* Handle error... */
		printf("could not open %s for writing!\n",filepath);
		return -1;
	}
	int result = settings_save(settings, f);
	fclose(f);
	if (result == 0)
	{
	    	/* Handle write error... */
		printf("unable to save settings!\n");
		return -1;
	}
	return 0;
}
/*****************************************************************************/
int ADSettings::get_version()
{
	return MetaData.version;
}
int ADSettings::set_version(int version)
{
	MetaData.version=version;
	return 0;
}
int ADSettings::get_seq_count(int* count)
{
	//MetaData.seq_count
	return 0;
}
int ADSettings::get_size(unsigned int *size)
{
	//MetaData.size
	return 0;
}
int ADSettings::get_checksum(unsigned int *cs)
{
	return 0;
}
int ADSettings::ReadStatus(char *StsMsg)
{
	const char *table[]   = SETTINGS_STATUS_STRING_TABLE;
	strcpy(StsMsg,table[SettingsState]);
	return 0;
}
/*****************************************************************************/
//bool ADSettings::VerifySha1Checksum(char* file,char* sha1sum,int sumlen)
//{
//	return true;
//}
int ADSettings::ReadMetaData(char* file,SETTINGS_META_DATA *pData)
{
	//init meta data struct before filling
	pData->version=0;
	pData->seq_count=0;
	pData->size =0;
	//pData->sha1hash[0]='\0';

	Settings *pMetaSettings=NULL;
	FILE *f;
	f = fopen(file, "r");
	if (f == NULL)
		return -1;
	pMetaSettings = settings_open(f);
	fclose(f);
	if (pMetaSettings == NULL)
		return -1;

	if(settings_get_int(pMetaSettings,SETTINGS_SECTION_META_DATA_NAME,SETTINGS_SECTION_META_DATA_VERSION  ,&pData->version)==0)
		pData->version=-1;
	if(settings_get_int(pMetaSettings,SETTINGS_SECTION_META_DATA_NAME,SETTINGS_SECTION_META_DATA_SEQ_COUNT,&pData->seq_count)==0)
		pData->seq_count=-1;
	if(settings_get_int(pMetaSettings,SETTINGS_SECTION_META_DATA_NAME,SETTINGS_SECTION_META_DATA_SIZE     ,&pData->size)==0)
		pData->size = -1;
//	if(settings_get    (pMetaSettings,SETTINGS_SECTION_META_DATA_NAME,SETTINGS_SECTION_META_DATA_CHECKSUM ,pData->sha1hash,sizeof(pData->sha1hash))==0)
//		pData->sha1hash[0]='\0';

	settings_delete(pMetaSettings);
	return 0;
}
/*****************************************************************************/
int ADSettings::InitAndInsertMetaDataEntries()//SETTINGS_META_DATA *pMetaData)
{
init_setting(&MetaStngTbl[0],SETTINGS_META_ENTRY_VERSION ,ESETTINGS_KEY_TYPE_INT   ,SETTINGS_SECTION_META_DATA_VERSION  ,(void*)&MetaData.version  ,sizeof(MetaData.version)  ,ESETTINGS_SECTION_META_DATA);
init_setting(&MetaStngTbl[1],SETTINGS_META_ENTRY_SEQCOUNT,ESETTINGS_KEY_TYPE_INT   ,SETTINGS_SECTION_META_DATA_SEQ_COUNT,(void*)&MetaData.seq_count,sizeof(MetaData.seq_count),ESETTINGS_SECTION_META_DATA);
init_setting(&MetaStngTbl[2],SETTINGS_META_ENTRY_SIZE    ,ESETTINGS_KEY_TYPE_INT   ,SETTINGS_SECTION_META_DATA_SIZE     ,(void*)&MetaData.size     ,sizeof(MetaData.size)     ,ESETTINGS_SECTION_META_DATA);
//init_setting(&MetaStngTbl[3],SETTINGS_META_ENTRY_SHA1HASH,ESETTINGS_KEY_TYPE_STRING,SETTINGS_SECTION_META_DATA_CHECKSUM ,(void*)MetaData.sha1hash  ,sizeof(MetaData.sha1hash) ,ESETTINGS_SECTION_META_DATA);
attach_setting(&MetaStngTbl[0]);
attach_setting(&MetaStngTbl[1]);
attach_setting(&MetaStngTbl[2]);
//attach_setting(&MetaStngTbl[3]);
	return 0;
}
int ADSettings::ValidateMetaDataEntries()
{
	if(MetaStngTbl[0].key_state != ESETTINGS_KEY_STATE_LOADED)
		MetaData.version=0;

	if(MetaStngTbl[1].key_state != ESETTINGS_KEY_STATE_LOADED)
		MetaData.seq_count=0;

	if(MetaStngTbl[2].key_state != ESETTINGS_KEY_STATE_LOADED)
		MetaData.size=0;

//	if(MetaStngTbl[3].key_state != ESETTINGS_KEY_STATE_LOADED)
//	{
//		MetaData.sha1hash[0]='0';
//		MetaData.sha1hash[1]='\0';
//	}
	return 0;
}
int ADSettings::UpdateSequenceCount()
{
	char value[1024];
	SETTINGS_ENTRY *pEntry,*pEntryLocal;

	if(settings_open_flag==false)
		return -1;

	pEntry=(SETTINGS_ENTRY*)SettingsChain.chain_get_by_ident(SETTINGS_META_ENTRY_SEQCOUNT);
	if(pEntry==NULL)
		return -1;

	pEntryLocal=(SETTINGS_ENTRY*)SettingsChainLocal.chain_get_by_ident(SETTINGS_META_ENTRY_SEQCOUNT);
	if(pEntryLocal==NULL)
		return -1;

	MetaData.seq_count++;
	*((int*)pEntryLocal->value) = *((int*)pEntry->value);
	sprintf(value,"%d",*((int*)pEntry->value));
	settings_set(settings,pEntry->section_name,pEntry->key,value);
	return 0;
}
int ADSettings::CalculateCheckSum(char* file)
{
	char cmd[1024];
	//sha1sum data.txt > data.txt.cs
	sprintf(cmd,"sha1sum %s > %s.%s",file,file,SHA1_FILE_EXTENSION);//with .prerix, hide cs file
	return system(cmd);
}
bool ADSettings::VerifyCheckSum(char* file)
{
	char cmd[1024];
	sprintf(cmd,"%s.sha1",file);
	if(Is_it_valid_file(cmd)!=0)
		return false;

	//sha1sum -c --quiet --status data.txt.cs
	sprintf(cmd,"sha1sum -c %s.%s >/dev/null 2>&1",file,SHA1_FILE_EXTENSION);//with .prerix, hide cs file
	if( system(cmd) == 0)
		return true;
	else
		return false;
}
int ADSettings::SwapAlternateFile(SETTINGS_META_DATA *pMetaData)
{
	char tmpfile[1024];
	strcpy(tmpfile,pMetaData->alternate_file);
	strcpy(pMetaData->alternate_file,pMetaData->file);
	strcpy(pMetaData->file,tmpfile);
	if(pMetaData->selected_file==ESETTINGS_SELECTED_FILE_MAIN)
		pMetaData->selected_file=ESETTINGS_SELECTED_FILE_BACKUP;
	else
		pMetaData->selected_file=ESETTINGS_SELECTED_FILE_MAIN;
	return 0;
}
/*****************************************************************************/
int ADSettings::FileConsistencyCheck(char* mainfile,SETTINGS_META_DATA *pMetaData)
{
	char bkupfile[1024];
	char cmd[1024];

	if(strlen(mainfile)==0)
	{
		SettingsState=ESETTINGS_STATE_NO_FILE_PASSED;
		return -1;//no settings file passed
	}
	
	//check and create main file if not available
	if(Is_it_valid_file(mainfile)!=0)
	{
		SettingsState=ESETTINGS_STATE_EMPTY_FILE_CREATE_ERROR;
		//user passed filename, but file doesnt exist, hence create a new file
		sprintf(cmd,"touch %s",mainfile);
		if(system(cmd)!=0)
			return -1;//unable to create a blank file
		SettingsState=ESETTINGS_STATE_EMPTY_FILE_DEFAULT_DATA;
	}

	//check and create backup file if not available
	sprintf(bkupfile,"%s.%s",mainfile,ALTERNATE_FILE_EXTENSION);
	if(Is_it_valid_file(bkupfile)!=0)
	{
		//user passed filename, but file doesnt exist, hence create a new file
		sprintf(cmd,"touch %s",bkupfile);
		if(system(cmd)!=0)
			return -1;//unable to create a blank file
	}

	SETTINGS_META_DATA Metamain,Metabkup;
	ReadMetaData(mainfile,&Metamain);
	ReadMetaData(bkupfile,&Metabkup);

	bool CSMainfile = VerifyCheckSum(mainfile);//VerifySha1Checksum(mainfile,Metamain.sha1hash,51);
	bool CSBkupfile = VerifyCheckSum(bkupfile);//VerifySha1Checksum(bkupfile,Metabkup.sha1hash,51);

	if(CSMainfile==false && CSBkupfile==false)
	{
		//select backup so that next save will be on mainfile
		pMetaData->selected_file=ESETTINGS_SELECTED_FILE_BACKUP;
		strcpy(pMetaData->file,bkupfile);
		strcpy(pMetaData->alternate_file,mainfile);
		//pMetaData->selected_file=ESETTINGS_SELECTED_FILE_NONE;
		//printf("both files are corrupt\n");

		if(SettingsState!=ESETTINGS_STATE_EMPTY_FILE_DEFAULT_DATA)
			SettingsState=ESETTINGS_STATE_BOTH_FILES_CORRUPT;

		return 0;//both files are corrupt, caller will use default values
	}
	if(CSMainfile==true && CSBkupfile==false)
	{
		pMetaData->selected_file=ESETTINGS_SELECTED_FILE_MAIN;
		strcpy(pMetaData->file,mainfile);
		strcpy(pMetaData->alternate_file,bkupfile);
		//printf("mainfile is healthy\n");

		if(SettingsState!=ESETTINGS_STATE_EMPTY_FILE_DEFAULT_DATA)
			SettingsState=ESETTINGS_STATE_BACKUP_FILE_CORRUPT;

		return 0;//mainfile is healthy, let caller use this
	}
	if(CSMainfile==false && CSBkupfile==true)
	{
		pMetaData->selected_file=ESETTINGS_SELECTED_FILE_BACKUP;
		strcpy(pMetaData->file,bkupfile);
		strcpy(pMetaData->alternate_file,mainfile);
		//printf("backupfile is healthy\n");

		if(SettingsState!=ESETTINGS_STATE_EMPTY_FILE_DEFAULT_DATA)
			SettingsState=ESETTINGS_STATE_MAIN_FILE_CORRUPT;


		return 0;//bkupfile is healthy, let caller use this
	}
	//printf("both files are healthy\n");


	//checksum of both main and backup are OK, now get the latest-updated file
	if(Metamain.seq_count>=Metabkup.seq_count)
	{
		pMetaData->seq_count = Metamain.seq_count;
		pMetaData->version   = Metamain.version;
		pMetaData->size      = Metamain.size;
		strcpy(pMetaData->file,mainfile);
		strcpy(pMetaData->alternate_file,bkupfile);
		pMetaData->selected_file=ESETTINGS_SELECTED_FILE_MAIN;
		//printf("selected mainfile\n");
		SettingsState=ESETTINGS_STATE_OK_MAIN;
		return 0;
	}
	else
	{
		pMetaData->seq_count = Metabkup.seq_count;
		pMetaData->version   = Metabkup.version;
		pMetaData->size      = Metabkup.size;
		strcpy(pMetaData->file,bkupfile);
		strcpy(pMetaData->alternate_file,mainfile);
		pMetaData->selected_file=ESETTINGS_SELECTED_FILE_BACKUP;
		//printf("selected backupfile\n");
		SettingsState=ESETTINGS_STATE_OK_BACKUP;
		return 0;//bkupfile is healthy, let caller use this
	}
	return 0;
}
//loads all the settings from text file into linked-list
int ADSettings::Load(char* file)
{
	//strcpy(filepath,file);
	filepath[0]='\0';
	MetaData.file[0]='\0';
	MetaData.alternate_file[0]='\0';

	if(FileConsistencyCheck(file,&MetaData)!=0)
		return -1;
	strcpy(filepath,MetaData.file);//open function will access global string filepath

	//initialized and preapre internal metadata variable entries
	InitAndInsertMetaDataEntries();//&MetaData);

	//if(strlen(file)==0)
	//	return -1;//no settings file passed
	//if(Is_it_valid_file(filepath)!=0)
	//{
		//user passed filename, but file doesnt exist, hence create a new file
	//	char cmd[1024];
	//	sprintf(cmd,"touch %s",filepath);
	//	if(system(cmd)!=0)
	//		return -1;//unable to create a blank file
	//}
	if(Open()!=0)
		return -1;
	int len=SettingsChain.get_chain_size();
	SETTINGS_ENTRY *pEntry,*pEntryLocal;
	int result=0;
	for(int i=0;i<len;i++)
	{
		pEntry=(SETTINGS_ENTRY*)SettingsChain.chain_get_by_index(i);
		if(pEntry==NULL)
			return -1;
		
		pEntryLocal=(SETTINGS_ENTRY*)SettingsChainLocal.chain_get_by_index(i);
		if(pEntryLocal==NULL)
			return -1;
		result=0;
		switch(pEntry->key_type)
		{
			case ESETTINGS_KEY_TYPE_STRING    :
				result =settings_get(settings,pEntry->section_name /*SETTINGS_SECTION_DATA_NAME*/, pEntry->key     ,(char*)pEntry->value     ,pEntry->value_size);
				result&=settings_get(settings,pEntryLocal->section_name/*SETTINGS_SECTION_DATA_NAME*/, pEntryLocal->key,(char*)pEntryLocal->value,pEntryLocal->value_size);
				break;
			case ESETTINGS_KEY_TYPE_INT       :
				result =settings_get_int(settings,pEntry->section_name/*SETTINGS_SECTION_DATA_NAME*/,pEntry->key     ,(int*)pEntry->value);
				result&=settings_get_int(settings,pEntryLocal->section_name/*SETTINGS_SECTION_DATA_NAME*/,pEntryLocal->key,(int*)pEntryLocal->value);
				break;
			case ESETTINGS_KEY_TYPE_INT_ARR   :
				result =settings_get_int_tuple(settings,pEntry->section_name/*SETTINGS_SECTION_DATA_NAME*/,pEntry->key     ,(int*)pEntry->value     ,(pEntry->value_size)/sizeof(int));
				result&=settings_get_int_tuple(settings,pEntryLocal->section_name/*SETTINGS_SECTION_DATA_NAME*/,pEntryLocal->key,(int*)pEntryLocal->value,(pEntryLocal->value_size)/sizeof(int));
				break;
			case ESETTINGS_KEY_TYPE_LONG      :
				result =settings_get_long(settings,pEntry->section_name/*SETTINGS_SECTION_DATA_NAME*/,pEntry->key     ,(long*)pEntry->value);
				result&=settings_get_long(settings,pEntryLocal->section_name/*SETTINGS_SECTION_DATA_NAME*/,pEntryLocal->key,(long*)pEntryLocal->value);
				break;
			case ESETTINGS_KEY_TYPE_LONG_ARR  :
				result =settings_get_long_tuple(settings,pEntry->section_name/*SETTINGS_SECTION_DATA_NAME*/,pEntry->key     ,(long*)pEntry->value     ,(pEntry->value_size)/sizeof(long));
				result&=settings_get_long_tuple(settings,pEntryLocal->section_name/*SETTINGS_SECTION_DATA_NAME*/,pEntryLocal->key,(long*)pEntryLocal->value,(pEntryLocal->value_size)/sizeof(long));
				break;
			case ESETTINGS_KEY_TYPE_DOUBLE    :
				result =settings_get_double(settings,pEntry->section_name/*SETTINGS_SECTION_DATA_NAME*/,pEntry->key     ,(double*)pEntry->value);
				result&=settings_get_double(settings,pEntryLocal->section_name/*SETTINGS_SECTION_DATA_NAME*/,pEntryLocal->key,(double*)pEntryLocal->value);
				break;
			case ESETTINGS_KEY_TYPE_DOUBLE_ARR:
				result =settings_get_double_tuple(settings,pEntry->section_name/*SETTINGS_SECTION_DATA_NAME*/,pEntry->key     ,(double*)pEntry->value     ,(pEntry->value_size)/sizeof(double));
				result&=settings_get_double_tuple(settings,pEntryLocal->section_name/*SETTINGS_SECTION_DATA_NAME*/,pEntryLocal->key,(double*)pEntryLocal->value,(pEntryLocal->value_size)/sizeof(double));
				break;
			default:break;
		}
		if (result == 0)
		{
			//printf("key %s not found\n",pEntry->key);
			pEntry->key_state      = ESETTINGS_KEY_STATE_NOT_FOUND;
			pEntryLocal->key_state = ESETTINGS_KEY_STATE_NOT_FOUND;
		}
		else
		{
			pEntry->key_state      = ESETTINGS_KEY_STATE_LOADED;
			pEntryLocal->key_state = ESETTINGS_KEY_STATE_LOADED;
		}
	}

	ValidateMetaDataEntries();
	return 0;
}
int ADSettings::Store()//store to last loaded file
{
	if(settings_open_flag==false)
	{
		//printf("settings file is not open!\n");
		return -1;
	}
	if(strlen(filepath)==0)
	{
		if (Load(filepath) !=0)
			return -1;//no settings file passed
	}
		//strcpy(filepath,file);
	char value[4096];//ensure that max size doesnt go above 1024 chars
	char temp_arr[512];//ensure that max size doesnt go above 1024 chars
	int len=SettingsChain.get_chain_size();
	SETTINGS_ENTRY *pEntry,*pEntryLocal;

	bool SettingsChanged=false;
	for(int i=0;i<len;i++)
	{
		pEntry=(SETTINGS_ENTRY*)SettingsChain.chain_get_by_index(i);
		if(pEntry==NULL)
			return -1;

		pEntryLocal=(SETTINGS_ENTRY*)SettingsChainLocal.chain_get_by_index(i);
		if(pEntryLocal==NULL)
			return -1;

		switch(pEntry->key_type)
		{
			case ESETTINGS_KEY_TYPE_STRING    :
				//printf("str val = %s\n", pEntry->value);
				sprintf(value,"%s",(char*)pEntry->value);
				if(strcmp((char*)pEntry->value,(char*)pEntryLocal->value)!=0){SettingsChanged=true;strcpy((char*)pEntryLocal->value,(char*)pEntry->value);/*printf("string changed\n");*/}
				break;
			case ESETTINGS_KEY_TYPE_INT       :
				sprintf(value,"%d",*((int*)pEntry->value));
				if( *((int*)pEntry->value) != *((int*)pEntryLocal->value) ) {SettingsChanged=true;*((int*)pEntryLocal->value) = *((int*)pEntry->value);/*printf("int changed\n");*/}
				break;
			case ESETTINGS_KEY_TYPE_INT_ARR   :
				{
					int *val=((int*)pEntry->value);
					int *localval=((int*)pEntryLocal->value);
					int len=pEntry->value_size/sizeof(int);
					value[0]='\0';
					for(int i=0;i<len;i++)
					{
						if(i == (len-1))
                					sprintf(temp_arr,"%d",*val);
						else        
        						sprintf(temp_arr,"%d, ",*val);

        					strcat(value,temp_arr);
						if( *val != *localval ) 
						{
							SettingsChanged=true;
							*localval=*val;
						}
						val++;
						localval++;
					}
				}
				break;
			case ESETTINGS_KEY_TYPE_LONG      :
				sprintf(value,"%ld",*((long*)pEntry->value));
				if( *((long*)pEntry->value) != *((long*)pEntryLocal->value) ){SettingsChanged=true;*((long*)pEntryLocal->value) = *((long*)pEntry->value);/*printf("long changed\n");*/}
				break;
			case ESETTINGS_KEY_TYPE_LONG_ARR  :
				{
					long *val=((long*)pEntry->value);
					long *localval=((long*)pEntryLocal->value);
					int len=pEntry->value_size/sizeof(long);
					value[0]='\0';
					for(int i=0;i<len;i++)
					{
						if(i == (len-1))
                					sprintf(temp_arr,"%ld",*val);
						else        
        						sprintf(temp_arr,"%ld, ",*val);

						strcat(value,temp_arr);
						if( *val != *localval ) 
						{
							SettingsChanged=true;
							*localval=*val;
						}
						val++;
						localval++;
					}
				}
				break;
			case ESETTINGS_KEY_TYPE_DOUBLE    :
				sprintf(value,"%E",*((double*)pEntry->value));
				if( *((double*)pEntry->value) != *((double*)pEntryLocal->value) ){SettingsChanged=true;*((double*)pEntryLocal->value) = *((double*)pEntry->value);/*printf("double changed\n");*/}
				break;
                        case ESETTINGS_KEY_TYPE_DOUBLE_ARR  :
				{
					double *val=((double*)pEntry->value);
					double *localval=((double*)pEntryLocal->value);
					int len=pEntry->value_size/sizeof(double);
					value[0]='\0';
					for(int i=0;i<len;i++)
					{
						if(i == (len-1))
                					sprintf(temp_arr,"%E",*val);
						else        
        						sprintf(temp_arr,"%E, ",*val);
						strcat(value,temp_arr);
						if( *val != *localval ) 
						{
							SettingsChanged=true;
							*localval=*val;
						}
						val++;
						localval++;
					}
					SettingsChanged=true;
				}
				break;
				
			//case ESETTINGS_KEY_TYPE_DOUBLE_ARR:break;//result=settings_get_double_tuple(settings,SETTINGS_SECTION_DATA_NAME,pEntry->key,(double*)pEntry->value,pEntry->value_size);break;
			default:sprintf(value,"unknown");break;
		}
		settings_set(settings,pEntry->section_name /*SETTINGS_SECTION_DATA_NAME*/,pEntry->key,value);
	}
	if(SettingsChanged==true)
	{
		strcpy(filepath,MetaData.alternate_file);//dont save back to where it was loaded, alternate the files
		UpdateSequenceCount();//increase the sequence count
		int result = Save();
		if(result != 0)
			return result;
		SwapAlternateFile(&MetaData);
		//printf("saved to %s\n",filepath);
		return CalculateCheckSum(filepath);
	}
	else
	{
		//printf("no need to save data\n");
		return 0;
	}
}
/*****************************************************************************/
//int init_setting(SETTINGS_ENTRY* pEntry,unsigned int indx,ESETTINGS_KEY_TYPE type,const char* name, void* val,size_t val_size,ESETTINGS_SECTION section=ESETTINGS_SECTION_DATA);
int ADSettings::init_setting(SETTINGS_ENTRY* pEntry,unsigned int indx,ESETTINGS_KEY_TYPE type,const char* name, void* val,size_t val_size,ESETTINGS_SECTION section)
{
	pEntry->key_index=indx;
	pEntry->key_type =type;
	strcpy(pEntry->key,name);
	pEntry->value = val;
	pEntry->value_size = val_size;
	pEntry->key_state = ESETTINGS_KEY_STATE_NOT_LOADED;
	pEntry->section=section;
	if(pEntry->section==ESETTINGS_SECTION_META_DATA)
		strcpy(pEntry->section_name,SETTINGS_SECTION_META_DATA_NAME);
	else
		strcpy(pEntry->section_name,SETTINGS_SECTION_DATA_NAME);
	return 0;
}
/*****************************************************************************/
int ADSettings::attach_setting(SETTINGS_ENTRY* pEntry)
{
	//create and store a local copy for comparing while save
	SETTINGS_ENTRY* pNewEntry=NULL;
	OBJECT_MEM_NEW(pNewEntry,SETTINGS_ENTRY);//delete this object in run_work() callback function
	if(pNewEntry==NULL)return -1;//something is wrong
	pNewEntry->key_index = pEntry->key_index;
	pNewEntry->key_type  = pEntry->key_type;
	strcpy(pNewEntry->key,pEntry->key);

	//pNewEntry->value     = pEntry->value;
	pNewEntry->value=NULL;
	ARRAY_MEM_NEW(pNewEntry->value,pEntry->value_size);

	pNewEntry->value_size= pEntry->value_size;
	pNewEntry->key_state = pEntry->key_state;
	pNewEntry->section   = pEntry->section;
	strcpy(pNewEntry->section_name,pEntry->section_name);

	if(SettingsChainLocal.chain_put((void *)pNewEntry)!=0)
	{
		//OBJ_MEM_DELETE(pNewEntry->value);
		//ARRAY_MEM_DELETE(pNewEntry->value);
		delete_value(pNewEntry->key_type,pNewEntry->value);
		OBJ_MEM_DELETE(pNewEntry);
		return -1;
	}

	if(SettingsChain.chain_put((void *)pEntry)!=0)
		return -1;
	return 0;
}
/*****************************************************************************/

