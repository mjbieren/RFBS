#!/bin/bash

#RFBS.out -f <FastaFileFolder> -h <HeaderFilePath> -r <OutputFolderPath> [-p]

PROGRAM_PATH=RFBS.out
FASTAFILES= #Folder containing fasta FILES
OUTPUT=	#Output Folder
HEADERFILEPATH= #Path to Header File


#Run where you remove specific sequences from sets
$PROGRAM_PATH -f $FASTAFILES -r $OUTPUT -h $HEADERFILEPATH


#Run where you remove specific sequences and reformat the fasta headers into Phylopypruner format <StrainName>@<GeneID>
#$PROGRAM_PATH -f $FASTAFILES -r $OUTPUT -h $HEADERFILEPATH -p