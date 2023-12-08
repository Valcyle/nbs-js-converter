/*    
    This project is started on 09/15/2023
    by Valcyle
    --You can read code for learning but dont use in wrong way!--
    -Valcyle[PRO7169], MCsmash Network (2023)
*/    
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

typedef struct fileinfo
{
    char nbsversion;
    char vanillainstrumentcount;
    short songlength;
    short layercount;
    char songname[1024];
    char auther[1024];
    char autheroriginal[1024];
    char description[1024];
    short tempo;
    char autosaving;
    char autosavingduration;
    char timesignature;
    int minutesspend;
    int clicks[2];
    int blocksedit[2];
    char schemafilename[1024];
    char loop;
    char maxloopcount;
    short loopstarttick;
} FileInfo;

//note sounds
const char sound[16][128] = {"harp", "bass", "bd", "snare", "hat", "guitar", "flute", "bell", "chime",
"xylophone", "iron_xylophone", "cow_bell", "didgeridoo", "bit", "banjo", "pling"};
//note pitch
const char pitch[25][128] = {"F#", "G", "G#", "A", "A#", "B", "C", "C#", "D", "D#", "E", "F", "F#2", 
"G2", "G#2", "A2", "A#2", "B2", "C2", "C#2", "D2", "D#2", "E2", "F2", "F#3"};


int main(int argc[], char* argv[]){
    FILE* nbsfile;
    FILE* fp2;
    FILE* musicjs;
    FileInfo info;
    char filename[1024];
    char songinfofile[1024];
    char musicjsfile[1024];
    size_t checks[21];
    int len;
    char* removestr;
    int filelen;
    bool tick = true, layer = true;
    //jsfile
    short jumptick, jumplayer;
    char notesound, notepitch;
    char skipbyte;
    short skipshort;
    int wait;
    float calcwait;

    //get the target file
    printf("Enter a target file name (ex. file.nbs)>> ");
    scanf("%s", filename);

    nbsfile = fopen(filename, "rb");
    
    removestr = strrchr(filename, '.');
    if (removestr != NULL) {
        *removestr = '\0';
    }

    //remove space and put underbar to filename
    filelen = strlen(filename);
    for(int i = 0; i < filelen; i++){
        if(filename[i] == ' '){
            filename[i] = '_';
        }
    }

    //make songinfo text file
    sprintf(songinfofile, "songinfo_%s.txt", filename);
    fp2 = fopen(songinfofile, "w");

    //check file validation
    if(nbsfile == NULL){
        printf("Target file doesn't exist!\n");
        fclose(nbsfile);
        fclose(fp2);
        exit(1);
    }
    
    //skip first 2 byte
    fseek(nbsfile, 2, SEEK_SET);
    //read header
    checks[0] = fread(&(info.nbsversion), sizeof(char), 1, nbsfile);
    checks[1] = fread(&(info.vanillainstrumentcount), sizeof(char), 1, nbsfile);
    checks[2] = fread(&(info.songlength), sizeof(short), 1, nbsfile);
    checks[3] = fread(&(info.layercount), sizeof(short), 1, nbsfile);
    fread(&len, sizeof(int), 1, nbsfile);
    checks[4] = fread(info.songname, sizeof(char), len, nbsfile);
    info.songname[checks[4]] = '\0';
    fread(&len, sizeof(int), 1, nbsfile);
    checks[5] = fread(info.auther, sizeof(char), len, nbsfile);
    info.auther[checks[5]] = '\0';
    fread(&len, sizeof(int), 1, nbsfile);
    checks[6] = fread(info.autheroriginal, sizeof(char), len, nbsfile);
    info.autheroriginal[checks[6]] = '\0';
    fread(&len, sizeof(int), 1, nbsfile);
    checks[7] = fread(info.description, sizeof(char), len, nbsfile);
    info.description[checks[7]] = '\0';
    checks[8] = fread(&(info.tempo), sizeof(short), 1, nbsfile);
    checks[9] = fread(&(info.autosaving), sizeof(char), 1, nbsfile);
    checks[10] = fread(&(info.autosavingduration), sizeof(char), 1, nbsfile);
    checks[11] = fread(&(info.timesignature), sizeof(char), 1, nbsfile);
    checks[12] = fread(&(info.minutesspend), sizeof(int), 1, nbsfile);
    checks[13] = fread(&(info.clicks[0]), sizeof(int), 1, nbsfile);
    checks[14] = fread(&(info.clicks[1]), sizeof(int), 1, nbsfile);
    checks[15] = fread(&(info.blocksedit[0]), sizeof(int), 1, nbsfile);
    checks[16] = fread(&(info.blocksedit[1]), sizeof(int), 1, nbsfile);
    fread(&len, sizeof(int), 1, nbsfile);
    checks[17] = fread(info.schemafilename, sizeof(char), len, nbsfile);
    info.schemafilename[checks[17]] = '\0';
    checks[18] = fread(&(info.loop), sizeof(char), 1, nbsfile);
    checks[19] = fread(&(info.maxloopcount), sizeof(char), 1, nbsfile);
    checks[20] = fread(&(info.loopstarttick), sizeof(short), 1, nbsfile);


    for(int i = 0; i < 21; i++){
        printf("%d\n", checks[i]);
    }

    //info print
    printf("nbsversion: %hhu\n", info.nbsversion);
    printf("vanilla_instrument_count: %hhu\n", info.vanillainstrumentcount);
    printf("length: %hd\n", info.songlength);
    printf("layercount: %hd\n", info.layercount);
    printf("songname: %s\n", info.songname);
    printf("auther: %s\n", info.auther);
    printf("autheroriginal: %s\n", info.autheroriginal);
    printf("description: %s\n", info.description);
    printf("tempo: %hd\n", info.tempo);
    printf("autosaving: %hhu\n", info.autosaving);
    printf("autosavingduration: %hhu\n", info.autosavingduration);
    printf("timesignature: %hhu\n", info.timesignature);
    printf("minutesspend: %d\n", info.minutesspend);
    printf("leftclick: %d\n", info.clicks[0]);
    printf("rightclick: %d\n", info.clicks[1]);
    printf("blocksadded: %d\n", info.blocksedit[0]);
    printf("blocksremoved: %d\n", info.blocksedit[1]);
    printf("schemafilename: %s\n", info.schemafilename);
    printf("loop: %hhu\n", info.loop);
    printf("maxloopcount: %hhu\n", info.maxloopcount);
    printf("loopstarttick: %hd\n", info.loopstarttick);

    //info write file
    fprintf(fp2, "nbsversion: %hhu\n", info.nbsversion);
    fprintf(fp2, "vanilla_instrument_count: %hhu\n", info.vanillainstrumentcount);
    fprintf(fp2, "length: %hd\n", info.songlength);
    fprintf(fp2, "layer_count: %hd\n", info.layercount);
    fprintf(fp2, "song_name: %s\n", info.songname);
    fprintf(fp2, "auther: %s\n", info.auther);
    fprintf(fp2, "auther_original: %s\n", info.autheroriginal);
    fprintf(fp2, "description: %s\n", info.description);
    fprintf(fp2, "tempo: %hd\n", info.tempo);
    fprintf(fp2, "auto_saving: %hhu\n", info.autosaving);
    fprintf(fp2, "auto_saving_duration: %hhu\n", info.autosavingduration);
    fprintf(fp2, "time_signature: %hhu\n", info.timesignature);
    fprintf(fp2, "minutes_spend: %d\n", info.minutesspend);
    fprintf(fp2, "left_click: %d\n", info.clicks[0]);
    fprintf(fp2, "right_click: %d\n", info.clicks[1]);
    fprintf(fp2, "blocks_added: %d\n", info.blocksedit[0]);
    fprintf(fp2, "blocks_removed: %d\n", info.blocksedit[1]);
    fprintf(fp2, "schema_filename: %s\n", info.schemafilename);
    fprintf(fp2, "loop: %hhu\n", info.loop);
    fprintf(fp2, "max_loop_count: %hhu\n", info.maxloopcount);
    fprintf(fp2, "loop_start_tick: %hd\n", info.loopstarttick);

    //make songinfo text file
    sprintf(musicjsfile, "%s.js", filename);
    musicjs = fopen(musicjsfile, "w");
    //song note reads and write
    fprintf(musicjs, "export const %s = [\n", filename);

    while (tick == true)
    {
        fread(&jumptick, sizeof(short), 1, nbsfile);
        if((int)jumptick > 0){
            fprintf(musicjs, "  ");
            printf("jump: %d\n", (int)jumptick);
            // if((info.tempo / 100) >= 18){
            //     calcwait = jumptick / 2.0; //10 //2
            // }else if((info.tempo / 100) >= 13 && (info.tempo / 100) < 18){
            //     calcwait = jumptick / 1.5; //15 //3
            // }else if((info.tempo / 100) >= 7 && (info.tempo / 100) < 13){
            //     calcwait = 2 * jumptick - 1; //19 //5
            // }else{
            //     calcwait = 3 * jumptick - 1; //29 //8
            // }
            calcwait = (20.0 / (info.tempo / 100)) * jumptick - 1;
            wait = roundf(calcwait);
            if((info.tempo / 100) < 17 && wait == 0){
                wait = 1;
            }
            
            for(int i = 0; i < wait; i++){
                fprintf(musicjs, "[],");
            }
            fprintf(musicjs, "\n");
        }else{
            tick = false;
            break;
        }
        layer = true;
        fprintf(musicjs, "  [\n");
        while (layer == true)
        {
            fread(&jumplayer, sizeof(short), 1, nbsfile);
            if((int)jumplayer > 0){
                fread(&notesound, sizeof(char), 1, nbsfile);
                fread(&notepitch, sizeof(char), 1, nbsfile);
                fread(&skipbyte, sizeof(char), 1, nbsfile);
                fread(&skipbyte, sizeof(unsigned char), 1, nbsfile);
                fread(&skipshort, sizeof(short), 1, nbsfile);
                printf("sound: %d, pitch: %d\n", (int)notesound, (int)notepitch);//

                //write
                if(notepitch >= 33 && notepitch <= 57 && notesound >= 0 && notesound <= 15){
                fprintf(musicjs, "      {type: \"%s\", pitch: \"%s\"},\n", sound[notesound], pitch[notepitch - 33]);
                printf("{type: \"%s\", pitch: \"%s\"},\n", sound[notesound], pitch[notepitch - 33]);//
                }
            }else{
                layer = false;
            }
        }
        fprintf(musicjs, "  ],\n");
    }
    fprintf(musicjs, "]");
    
    fclose(nbsfile);
    fclose(fp2);
    fclose(musicjs);
    return 0;
}