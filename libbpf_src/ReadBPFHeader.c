#include	<stdlib.h>
#include	<stdio.h>
#include	"libbpf.h"
#include	"libsize_types.h"

#define DEBUG(x) printf("HERE %d\n", x); fflush(stdout);
		
si4     ReadBPFHeader(ui1 *dp, ui4 *BPFRecordSizes, si2 *Gains, si4 *AcquisitionSystem, ui4 *EEGChList )
{
	ui1	VerifyRecordFormat(ui1 *, ui4 *, ui1 *, si1 *, si1 *, ui1);
	ui1	*FindHeaderStart(), *FindHeaderEnd(), *FindSection(), *FindSectionEnd();
	void	GetGainsList(), GetRecordSizes(); 
	ui1	BPFRecordTypes[256];
	ui1	*HeaderStart, *HeaderEnd, *SectionStart, *SectionEnd;
	ui4	BPFRecordTypeNumbers[256];
	si1	KeyWordString[256], Encoding[4096], **Parameters;
	si4	Type, NumberOfParameters, NumEEGChannels;
	si4	GetRecordTypesUsed(), FindKeyWord();

	// Look for header beginning
	HeaderStart = FindHeaderStart(dp);
	if(HeaderStart == NULL){
		fprintf(MyStdErr,"\nCan't find start of header (%s).\nWrong File Format.\n", HEADER_BEGIN_LABEL);
		return(-1);
	}
	dp = (HeaderStart + strlen(HEADER_BEGIN_LABEL));
	// Check to see if it's the BPF header or not
	sscanf((si1 *)dp,"%s", KeyWordString);
	if(strstr(KeyWordString, BPF_HEADER_TYPE) == NULL){
		fprintf(MyStdErr,"This is not a BPF file. It is a %s file.\n", KeyWordString);
		return(-1);
	}
        // Section: DATABASE_INFORMATION
        dp = HeaderStart;
        SectionStart = FindSection(dp, SECTION_DATABASE_INFORMATION);
        if(SectionStart == NULL){
                fprintf(MyStdErr, "\nCan't find Header section %s.\nWrong File Format.\n", SECTION_DATABASE_INFORMATION);
                return(-1);
        }

        dp = SectionStart;

        // Acquisition System
        *AcquisitionSystem = ACX;
        if(!FindKeyWord(SectionStart, ACQUISITION_SYSTEM_LABEL, KeyWordString, Encoding)){
                printf("AcX Acquisition System\n");
        }else{
                Type = GetType(KeyWordString);
                switch(Type){
                case -1:
                        fprintf(MyStdErr,"KeyWord.integer format not respected in: %s\nWrong File Format.\n", KeyWordString);
                        return(0);

                case 0: // ASCII encoded
                        NumberOfParameters = GetASCIIEncoding(Encoding, &Parameters);
                        if(NumberOfParameters == 1){
                                if(strcasestr(Parameters[0], "Axona") != '\0'){
                                        *AcquisitionSystem = AXONA;
                                        printf("AXONA Acquisition System\n");
                                }
                        }
                        break;

                default:
                        fprintf(MyStdErr,"%s must be ASCII encoded\nWrong File Format.\n", ACQUISITION_SYSTEM_LABEL);
                return(0);
                }
        }
	// Section: SETUP_INFORMATION
	dp = HeaderStart;
	SectionStart = FindSection(dp, SECTION_SETUP_INFORMATION);
	if(SectionStart == NULL){
		fprintf(MyStdErr, "\nCan't find Header section %s.\nWrong File Format.\n", SECTION_SETUP_INFORMATION);
		return(-1);
	}

        dp = SectionStart;
        NumEEGChannels = GetRecordTypesUsed(BPFRecordTypeNumbers, dp, EEGChList);

	GetGainsList(Gains, dp);

	SectionEnd = FindSectionEnd(SectionStart);
	if(SectionEnd == NULL)
		fprintf(MyStdErr,"\nCan't find end of Header Section (%s).\nWrong File Format.\n", SECTION_SETUP_INFORMATION);


	// Section: RECORD_FORMAT
	// don't set default record sizes because they were set already
	/*
	BPFRecordSizes[EEG_BPF_REC_TYPE] = EEG_BPF_REC_SIZE;
	BPFRecordSizes[SINGLE_BPF_REC_TYPE] = SINGLE_BPF_REC_SIZE;
	BPFRecordSizes[STEREO_BPF_REC_TYPE] = STEREO_BPF_REC_SIZE;
	BPFRecordSizes[TETRODE_BPF_REC_TYPE] = TETRODE_BPF_REC_SIZE;
	BPFRecordSizes[SYNC_BPF_REC_TYPE] = SYNC_BPF_REC_SIZE;
	BPFRecordSizes[ROOM_POSITION_BPF_REC_TYPE] = ROOM_POSITION_BPF_REC_SIZE;
	BPFRecordSizes[ARENA_POSITION_BPF_REC_TYPE] = ARENA_POSITION_BPF_REC_SIZE;
	*/
	dp = HeaderStart;
	SectionStart = FindSection(dp, SECTION_RECORD_FORMAT_INFORMATION);
	if(SectionStart == NULL){ 
		fprintf(MyStdErr, "\nCan't find section %s. Wrong data format\n", SECTION_RECORD_FORMAT_INFORMATION);
		return (-1);
	}

	// EEG RECORD ID
	if(BPFRecordTypeNumbers[EEG_BPF_REC_TYPE]){
		BPFRecordSizes[EEG_BPF_REC_TYPE] = BPFRecordTypeNumbers[EEG_BPF_REC_TYPE] * EEG_BPF_REC_DATA_SIZE + EEG_BPF_REC_ID_SIZE;
		if(!VerifyRecordFormat(BPFRecordTypes, BPFRecordSizes, SectionStart, EEG_ID, EEG_RECORD_FORMAT, EEG_BPF_REC_TYPE))
			return(-1);
	}
	// SINGLE ELECTRODE RECORD ID
	if(BPFRecordTypeNumbers[SINGLE_BPF_REC_TYPE]){
		if(!VerifyRecordFormat(BPFRecordTypes, BPFRecordSizes, SectionStart, SINGLE_ID, SINGLE_RECORD_FORMAT, SINGLE_BPF_REC_TYPE))
			return(-1);
	}

	// STEREOTRODE RECORD ID
	if(BPFRecordTypeNumbers[STEREO_BPF_REC_TYPE]){
		if(!VerifyRecordFormat(BPFRecordTypes, BPFRecordSizes, SectionStart, STEREO_ID, STEREO_RECORD_FORMAT, STEREO_BPF_REC_TYPE))
			return(-1);
	}
	// TETROTRODE RECORD ID
	if(BPFRecordTypeNumbers[TETRODE_BPF_REC_TYPE]){
		if(!VerifyRecordFormat(BPFRecordTypes, BPFRecordSizes, SectionStart, TETRODE_ID, TETRODE_RECORD_FORMAT, TETRODE_BPF_REC_TYPE))
			return(-1);
	}
	// SYNC RECORD ID
	if(BPFRecordTypeNumbers[SYNC_BPF_REC_TYPE]){
		if(!VerifyRecordFormat(BPFRecordTypes, BPFRecordSizes, SectionStart, SYNC_ID, SYNC_RECORD_FORMAT, SYNC_BPF_REC_TYPE))
			return(-1);
	}
	// ROOM POSITION RECORD ID
	if(BPFRecordTypeNumbers[ROOM_POSITION_BPF_REC_TYPE]){
		if(!VerifyRecordFormat(BPFRecordTypes, BPFRecordSizes, SectionStart, ROOM_POSITION_ID, ROOM_POSITION_RECORD_FORMAT, ROOM_POSITION_BPF_REC_TYPE))
			return(-1);
	}
	// ARENA POSITION RECORD ID
	if(BPFRecordTypeNumbers[ARENA_POSITION_BPF_REC_TYPE]){
		if(!VerifyRecordFormat(BPFRecordTypes, BPFRecordSizes, SectionStart, ARENA_POSITION_ID, ARENA_POSITION_RECORD_FORMAT, ARENA_POSITION_BPF_REC_TYPE))
			return(-1);
	}
        // KEY EVENT RECORD ID
       //  if(BPFRecordTypeNumbers[KEY_EVENT_BPF_REC_TYPE]){
                VerifyRecordFormat(BPFRecordTypes, BPFRecordSizes, SectionStart, KEY_EVENT_ID, KEY_EVENT_RECORD_FORMAT, KEY_EVENT_BPF_REC_TYPE);
		// This record may not be defined
        // }
        // INPUT EVENT RECORD ID
        // if(BPFRecordTypeNumbers[INPUT_EVENT_BPF_REC_TYPE]){
                VerifyRecordFormat(BPFRecordTypes, BPFRecordSizes, SectionStart, INPUT_EVENT_ID, INPUT_EVENT_RECORD_FORMAT, INPUT_EVENT_BPF_REC_TYPE);
		// This record may not be defined
        // }
        // OUTPUT EVENT RECORD ID
        // if(BPFRecordTypeNumbers[OUTPUT_EVENT_BPF_REC_TYPE]){
                VerifyRecordFormat(BPFRecordTypes, BPFRecordSizes, SectionStart, OUTPUT_EVENT_ID, OUTPUT_EVENT_RECORD_FORMAT, OUTPUT_EVENT_BPF_REC_TYPE);
		// This record may not be defined
        //}
	SectionEnd = FindSectionEnd(SectionStart);
	if(SectionEnd == NULL)
		fprintf(MyStdErr,"\nCan't find end of Header Section (%s).\nWrong File Format.\n", SECTION_RECORD_FORMAT_INFORMATION);

	HeaderEnd = FindHeaderEnd(SectionEnd);
	if(HeaderEnd == NULL){
		fprintf(MyStdErr, "\nCan't find end of header. Wrong data format\n");
		return (-1);
	}
	return(1);
}

ui1     *FindHeaderStart(ui1 *data)
{
        si1 String[256], Line[4096], *HeaderStart;
	ui1 *dp;

	dp = data;
        while(sscanf((si1 *)dp, "%s", String) == 1){
                HeaderStart = (si1 *)strstr(String, HEADER_BEGIN_LABEL);
		if(HeaderStart != NULL){
                        return(dp + (HeaderStart - String));
                }       
        	sscanfLine(dp, Line, 4096);
                dp += strlen(Line);
        }       

        fprintf(MyStdErr,"Can't find BPF Header begin label: %s\n", HEADER_BEGIN_LABEL);
	return(NULL);
}       

ui1	*FindHeaderEnd(ui1 *data)
{
	si1 String[256], Line[4096];
	ui1 *dp;
	
	dp = data;
	
	while(sscanfLine((si1 *)dp, Line, 4096) != '\0'){
		dp = (ui1 *)strstr((si1 *)dp, Line);
		dp += strlen(Line);
		sscanf(Line, "%s", String);
		if(strstr(String, HEADER_END_LABEL) != NULL){
			return(dp);
		}
	}
	
	fprintf(MyStdErr,"Can't find BPF Header end label: %s\n", HEADER_END_LABEL);
	return(NULL);
}

ui1	*FindSection(ui1 *dp, ui1 *Section)
{
	si1	Flag[256];

	while (sscanf((si1 *)dp,"%s", Flag) != EOF){
		dp = (ui1 *)strstr((si1 *)dp, Flag);
		dp += strlen(Flag);
		if(Comment((si1 *)dp, Flag)) continue;

		if(!strcmp(Flag, SECTION_BEGIN_LABEL)){
			sscanf((si1 *)dp,"%s", Flag);
			dp = (ui1 *)strstr((si1 *)dp, Flag);
			dp += strlen(Flag);

			if(!strcmp(Flag, (si1 *)Section))
				return(dp);
		}
	}
	return NULL;
}

si4	FindKeyWord(si1 *dp, si1 *KeyWord, si1 *KeyWordString, si1 *Encoding)
{
	si4	SectionEnd(), n_chars;

	while (sscanf(dp,"%s", KeyWordString) != EOF){
		dp = (si1 *)strstr(dp, KeyWordString);
		dp += strlen(KeyWordString);
		if(Comment(dp, KeyWordString)) continue;
		if(SectionEnd(KeyWordString)) break;

		if(strstr(KeyWordString, KeyWord) != NULL){
			// search until the "." for a '(' Do this because some BPF files don't have a space between the Keyword and the encoding info. 
			// for example ListOfGains.0(2 2 2 2 ...
			n_chars = 0;
			while(*(dp-n_chars) != '.'){
				if(*(dp-n_chars) == '('){
					dp -= n_chars;
					break;
				}
				n_chars++;
			}
	
			(void)sscanfLine(dp, Encoding, 4096);
			return(1);
		}
	}
	return 0;
}

ui1	*FindSectionEnd(ui1 *dp)
{
	si1	Line[4096], String[4096];

	while (sscanfLine(dp, Line, 4096) != EOF){
		dp = (ui1 *)strstr((si1 *)dp, Line);
		dp+= strlen(Line);
		sscanf(Line,"%s",String);
		if(SectionEnd(String)){
			return(dp);
		}
	}
	return(NULL);
}

si4	Comment(ui1 *dp, si1 *String)
{
	si1	Line[4096];

	if(!strcmp(String, COMMENT)){
		sscanfLine(dp, Line, 4096);
		dp += strlen(Line);
		return(1);
	}else	
		return(0);
}

si4	SectionEnd(si1 *String)
{
	if(!strcmp(String, SECTION_END_LABEL))
		return(1);
	return(0);
}

si4	GetType(si1 *KeyWord)
{
	si1 *TypeString;
	si4	Type = -1;
	
	TypeString = (si1 *)strchr(KeyWord, TYPE_PREFIX);
	if(TypeString == NULL)
		return(-1);
	
	sscanf(TypeString+1,"%d", &Type);	// +1 to step past the '.'
	return(Type);
}

si4	GetBinaryEncoding(si1 *Encoding, si1 *P)
{ 
	static si1 String[256], *TerminateString, *Line;

	//  The character pair (' must start the Binary Encoding
	if((Line = (si1 *)strstr(Encoding, BINARY_STARTING_MARK)) == NULL){
		fprintf(MyStdErr,"a (' must start binary encodings.\n");
		return (0);
	}
	Line+= strlen(BINARY_STARTING_MARK);;
	sscanf(Line,"%c",P);
	if(P == NULL){
		fprintf(MyStdErr,"Can't read Binary code.\n");
		return (0);
	}
	return(1);
}

si4	GetASCIIEncoding(si1 *Encoding, si1 ***P)
{
	si4 n; 
	static si1 String[256], *TerminateString, *Line, **Parameters;

	
	Parameters = (si1 **)calloc(MAX_NUMBER_BPF_FORMAT_PARAMETERS, sizeof(si1 *));
	if(Parameters == NULL){
		fprintf(MyStdErr, "\nCan't allocate Parameters\n");
		return (0);
	}
	*P = Parameters;

	// a '(' must start the ASCII Encoding
	if((Line = (si1 *)strstr(Encoding, ASCII_STARTING_MARK )) == NULL){
		fprintf(MyStdErr,"a '(' must start ASCII encodings. Found: %s\n", Encoding);
		return (0);
	}
	Line+= strlen(ASCII_STARTING_MARK);;

	if(sscanf(Line, "%s", String) != 1){
		fprintf(MyStdErr,"No ASCII string to decode\n");
		return (0);
	}

	Parameters[0] = (si1 *)calloc(strlen(String + 1), sizeof(si1));
	strcpy(Parameters[0], String);

	if((TerminateString = (si1 *)strstr(Parameters[0], ASCII_TERMINATING_MARK )) != NULL){	// a ')' ends the encoding
		*TerminateString = '\0';
		return(1);
	}

	n = 1;

	Line += (strlen(String) + 1);
	while((*Line == ' ') || (*Line == '\t')){	// move to start of next string
		Line ++;
	}
	while(sscanf(Line, "%s", String) != EOF){
		if(!strcmp(String, ")"))
			return (n);
		Parameters[n] = (si1 *)calloc(strlen(String) + 1, sizeof(si1));
		strcpy(Parameters[n], String);
		if((TerminateString = (si1 *)strchr(Parameters[n], ')')) != NULL){ // a ')' indicates the end of the ASCII encoding
			*TerminateString = '\0' ;
			return(++n);
		}
		Line += (strlen(String) + 1);
		while((*Line == ' ') || (*Line == '\t')){	// move to start of next string
			Line ++;
		}
		n++;

		if(n == MAX_NUMBER_BPF_FORMAT_PARAMETERS){
			fprintf(MyStdErr,"Too many Parameters in Encoding of:\t");
			return(0);
		}
	}

	return (n);
}

ui1	VerifyRecordFormat(ui1 *RecordTypes, ui4 *RecordSizes, ui1 *SectionStart, si1 *id, si1 *RecordFormat, ui1 RecType){

	ui1	*dp;
	si1	KeyWordString[256], Encoding[256];
	si4	Type;
	
	// Get record type
        dp = SectionStart;
        if(!FindKeyWord((si1 *)dp, id, KeyWordString, Encoding)){
		if(!strcmp(id, KEY_EVENT_ID) && !strcmp(id, INPUT_EVENT_ID) && !strcmp(id, OUTPUT_EVENT_ID))
                	fprintf(MyStdErr,"\nCan't find keyWord (%s).\nWrong File Format.\n", id);
                return 0;
        }
        Type = GetType(KeyWordString);   
        switch(Type){   
        case -1:        
                fprintf(MyStdErr,"KeyWord.integer format not respected in: %s\nWrong File Format.\n", KeyWordString);
                return 0;
                                
        case 0: // ASCII encoded
                fprintf(MyStdErr,"%s\nWrong File Format.\n", id);
                return 0;       

        case RECORD_ID_ENCODING: // 1 byte Binary encoded
                if(!GetBinaryEncoding(Encoding, (si1 *)RecordTypes+RecType)){
                        fprintf(MyStdErr,"\nCan't get RECORD ID from header.\n");
                        return 0;
                }
                if(RecordTypes[RecType] != RecType){
                        fprintf(MyStdErr,"\nRecord type (%c) does not match line in  the header.\n", RecType);
                        return(0);
                }
                break;

        default:
                fprintf(MyStdErr,"Record id (%s) must be Binary encoded\nWrong File Format.\n", Type, KeyWordString);
                return (0);
        }

	//Get record size
        dp = SectionStart;
        if(!FindKeyWord((si1 *)dp, RecordFormat, KeyWordString, Encoding)){
                fprintf(MyStdErr,"\nCan't find keyWord (%s).\nWrong File Format.\n", RecordFormat);
                return 0;
        }
	RecordSizes[RecType] = (ui4)GetType(KeyWordString); 
	/* if (RecordSizes[RecType] != (ui4)GetType(KeyWordString)){ 
               fprintf(MyStdErr,"\nRecord size (%d) does not match line in the header (%s).\n", RecordSizes[RecType], KeyWordString);
		return(0);
	}
	*/
	return(1);
}
si4     GetRecordTypesUsed(ui4 *RecordTypeNumbers, si1 *SectionStart, ui4 *BPFEEGChannels){

        ui4     GetNumberOfChannels(), GetIfPositions();
        si1     *dp;
        ui4     i, n;
        si4     NumEEGChannels;
        si4     GetEEGChannelList();

        for(i = 0; i < MAX_BPF_RECORD_TYPES; i++)
                RecordTypeNumbers[i] = 0;

        n = 0;
        dp = SectionStart;
        n += GetNumberOfChannels(RecordTypeNumbers, dp, NUMBER_OF_EEG_CHANNELS, EEG_BPF_REC_TYPE);
        if(n){  // There are EEG channels
                NumEEGChannels = GetEEGChannelList(BPFEEGChannels, dp);
        }else
                BPFEEGChannels = NULL;
        n += GetNumberOfChannels(RecordTypeNumbers, dp, NUMBER_OF_SINGLE_CHANNELS, SINGLE_BPF_REC_TYPE);
        n += GetNumberOfChannels(RecordTypeNumbers, dp, NUMBER_OF_STEREO_CHANNELS, STEREO_BPF_REC_TYPE);
        n += GetNumberOfChannels(RecordTypeNumbers, dp, NUMBER_OF_TETRODE_CHANNELS, TETRODE_BPF_REC_TYPE);
        if(!n){
                fprintf(MyStdErr,"\nNo indication in header that there are electrophysiological data.\nWrong File Format.\n");
                return (NumEEGChannels);
        }

        (void) GetNumberOfChannels(RecordTypeNumbers, dp, NUMBER_OF_SYNC_CHANNELS, SYNC_BPF_REC_TYPE);
        (void) GetIfPositions(RecordTypeNumbers, dp, ROOM_POSITIONS, ROOM_POSITION_BPF_REC_TYPE);
        (void) GetIfPositions(RecordTypeNumbers, dp, ARENA_POSITIONS, ARENA_POSITION_BPF_REC_TYPE);

        return (NumEEGChannels);
}

si4	GetEEGChannelList(ui4 *BPFEEGChannels, si1 *SectionStart){
	ui1	*dp;
	si1	KeyWordString[256], Encoding[512], **Parameters;
	si4	NumberOfParameters, Type, i;

	dp = (ui1 *)SectionStart;
        if(!FindKeyWord((si1 *)dp, LIST_OF_EEG_CHANNELS, KeyWordString, Encoding)){
                fprintf(MyStdErr,"\nCan't find keyWord (%s).\n", LIST_OF_EEG_CHANNELS);
			return(-1);
        }
        Type = GetType(KeyWordString);
        switch(Type){
        case -1:
                fprintf(MyStdErr,"KeyWord.integer format not respected in: %s\nWrong File Format.\n", KeyWordString);
                return(-1); 

        case 0: // ASCII encoded
                NumberOfParameters = GetASCIIEncoding(Encoding, &Parameters);
		if(NumberOfParameters){
			for(i=0; i < NumberOfParameters; i++){
                                BPFEEGChannels[i] = atoi(Parameters[i]) - 1; // The header count begins at 1 but the channel count begins at 0 for processing
			}
		}
                return NumberOfParameters;

        default:
                fprintf(MyStdErr,"%s must be ASCII encoded\nWrong File Format.\n", KeyWordString);
                return 0;
        }
}	

ui4	GetNumberOfChannels(BPFRecordTypeNumbers, SectionStart, KeyWord, RecType)
	ui4 *BPFRecordTypeNumbers;
	ui1 *SectionStart;
	si1* KeyWord;
	ui1 RecType;
{
	ui1	*dp;
	si1	KeyWordString[256], Encoding[512], **Parameters;
	si4	Type, NumberOfParameters;

        dp = SectionStart;
        if(!FindKeyWord((si1 *)dp, KeyWord, KeyWordString, Encoding)){
                fprintf(MyStdErr,"\nCan't find keyWord (%s).\n", KeyWord);
			return(-1);
        }

        Type = GetType(KeyWordString);
        switch(Type){
        case -1:
                fprintf(MyStdErr,"KeyWord.integer format not respected in: %s\nWrong File Format.\n", KeyWordString);
                return(0);   

        case 0: // ASCII encoded
                NumberOfParameters = GetASCIIEncoding(Encoding, &Parameters);
                if(NumberOfParameters != 1){    
                        fprintf(MyStdErr,"(%s).\nWrong File Format.\n", KeyWord);
                        return 0;
                }
                sscanf(Parameters[0],"%d", &(BPFRecordTypeNumbers[RecType]));
                return(1);

        default:
                fprintf(MyStdErr,"%s must be ASCII encoded\nWrong File Format.\n", KeyWord);
                return(0);
        }
}	

ui4	GetIfPositions(BPFRecordTypeNumbers, SectionStart, KeyWord, RecType)
	ui4 *BPFRecordTypeNumbers;
	ui1 *SectionStart;
	si1* KeyWord;
	ui1 RecType;
{

	ui1	*dp;
	si1	KeyWordString[256], Encoding[512], **Parameters;
	si4	Type, NumberOfParameters;

        dp = SectionStart;
        if(!FindKeyWord((si1 *)dp, KeyWord, KeyWordString, Encoding)){
            // fprintf(MyStdErr,"Can't find keyWord (%s). This is not critical.\n", KeyWord);
			BPFRecordTypeNumbers[RecType] = 0;
            return 1;
        }

        Type = GetType(KeyWordString);
        switch(Type){
        case -1:
                fprintf(MyStdErr,"KeyWord.integer format not respected in: %s\nWrong File Format.\n", KeyWordString);
                return(0);   

        case 0: // ASCII encoded
                NumberOfParameters = GetASCIIEncoding(Encoding, &Parameters);
                if(NumberOfParameters != 1){    
                        fprintf(MyStdErr,"(%s).\nWrong File Format.\n", KeyWord);
                        return 0;
                }
                sscanf(Parameters[0],"%d", &(BPFRecordTypeNumbers[RecType]));
                return(1);

        default:
                fprintf(MyStdErr,"%s must be ASCII encoded\nWrong File Format.\n", KeyWord);
                return(0);
        }
}	

void	GetGainsList(si2 *Gains, ui1 *SectionStart){
	ui1	*dp;
	si1	KeyWordString[256], Encoding[512], **Parameters;
	si4	NumberOfParameters, Type, i;

	dp = SectionStart;
        if(!FindKeyWord((si1 *)dp, LIST_OF_GAINS, KeyWordString, Encoding)){
                fprintf(MyStdErr,"\nCan't find keyWord (%s).\n", LIST_OF_GAINS);
			return;
        }
        Type = GetType(KeyWordString);
        switch(Type){
        case -1:
                fprintf(MyStdErr,"KeyWord.integer format not respected in: %s\nWrong File Format.\n", KeyWordString);
                return; 

        case 0: // ASCII encoded
                NumberOfParameters = GetASCIIEncoding(Encoding, &Parameters);
		if(NumberOfParameters){
			// Gains = (ui2 *)calloc(MAX_NUMBER_OF_BPF_CHANNELS, sizeof(si2));
			for(i=0; i < NumberOfParameters; i++){
                		// sscanf(Parameters[i],"%d", &(Gains[i]));
                		Gains[i] = atoi(Parameters[i]);
			}
		}
                return;

        default:
                fprintf(MyStdErr,"%s must be ASCII encoded\nWrong File Format.\n", KeyWordString);
	        return;
        }
}
