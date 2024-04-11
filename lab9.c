/*
COP 3502
Matthew Ramaswami
Sect 0027
11 April 2024
*/

#include <stdio.h>
#include <stdlib.h>


// RecordType
struct RecordType
{
    int id;
    char name;
    int order;
    struct RecordType *next;  //added to record type so it can the next record can be accessed 
};

// Fill out this structure
struct HashType
{
	struct RecordType **records; //pointer to records
};

// Compute the hash function
int hash(int x, int hashSz)
{
	return x % hashSz; //hash function with x modulo hashSize
}

// parses input file to an integer array
int parseData(char* inputFileName, struct RecordType** ppData)
{
	FILE* inFile = fopen(inputFileName, "r");
	int dataSz = 0;
	int i, n;
	char c;
	struct RecordType *pRecord;
	*ppData = NULL;

	if (inFile)
	{
		fscanf(inFile, "%d\n", &dataSz);
		*ppData = (struct RecordType*) malloc(sizeof(struct RecordType) * dataSz);
		// Implement parse data block
		if (*ppData == NULL)
		{
			printf("Cannot allocate memory\n");
			exit(-1);
		}
		for (i = 0; i < dataSz; ++i)
		{
			pRecord = *ppData + i;
			fscanf(inFile, "%d ", &n);
			pRecord->id = n;
			fscanf(inFile, "%c ", &c);
			pRecord->name = c;
			fscanf(inFile, "%d ", &n);
			pRecord->order = n;
		}

		fclose(inFile);
	}

	return dataSz;
}

// prints the records
void printRecords(struct RecordType pData[], int dataSz)
{
	int i;
	printf("\nRecords:\n");
	for (i = 0; i < dataSz; ++i)
	{
		printf("\t%d %c %d\n", pData[i].id, pData[i].name, pData[i].order);
	}
	printf("\n\n");
}

// display records in the hash structure
// skip the indices which are free
// the output will be in the format:
// index x -> id, name, order -> id, name, order ....
void displayRecordsInHash(struct HashType *pHashArray, int hashSz)
{
	int i;
	struct RecordType *record;
	for (i=0;i<hashSz;++i)
	{
		// if index is occupied with any records, print all
		printf("Index %d -> ", i);
        if (pHashArray->records[i] != NULL)
        {
            record = pHashArray->records[i];
            while (record != NULL)
            {
                printf("%d, %c, %d -> ", record->id, record->name, record->order);
                record = record->next;
            }
        }
        printf("\n");
	}
}

int main(void)
{
	struct RecordType *pRecords;
	int recordSz = 0;
	int hashSz = 23;
	recordSz = parseData("input.txt", &pRecords);
	printRecords(pRecords, recordSz);
	// Your hash implementation

    struct HashType hashTable;
    hashTable.records = (struct RecordType **)malloc(sizeof(struct RecordType *) * hashSz);
    for (int i = 0; i < hashSz; i++)
    {
        hashTable.records[i] = NULL;
    }

    //hashing and inserting into hash table
    for (int i = 0; i < recordSz; i++)
    {
        int hashedIndex = hash(pRecords[i].id, hashSz);
        pRecords[i].next = hashTable.records[hashedIndex];
        hashTable.records[hashedIndex] = &pRecords[i];
    }

    //output of hashtable
    displayRecordsInHash(&hashTable, hashSz);

    //its free real estate
    for (int i = 0; i < recordSz; i++)
    {
        free(pRecords[i].next);
    }
    free(pRecords);
    free(hashTable.records);

	return 0;
}
