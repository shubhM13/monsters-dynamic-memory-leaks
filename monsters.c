#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "leak-detector-c.h"

typedef struct monster {
int id;
char *name;
char *element;
int population;
} monster;

typedef struct region {
char *name;
int nmonsters;
int total_population;
monster **monsters;
} region;

typedef struct itinerary {
int nregions;
region **regions;
int captures;
} itinerary;

typedef struct trainer {
char *name;
itinerary *visits;
} trainer;


/* Fill a monster. */
void fill_monster(monster *m, int id, char *name, char *element, int population)
{
    /* strdup allocates and strcpy's our strings all at once. */
    m->id = id;
    m->name = strdup(name);
    m->element = strdup(element);
    m->population = population;
}

// Fill a region
void fill_region(region *r, char *name, int nmonsters, int total_population, monster ** monsters)
{
    /* strdup allocates and strcpy's our strings all at once. */
    r->name = strdup(name);
    r->nmonsters = nmonsters;
    r->total_population = total_population;
    r->monsters = monsters;
}

// Fill a itinerary
void fill_itinerary(itinerary *it, int nregions, region **regions, int captures)
{
    it->nregions = nregions;
    it->regions = regions;
    it->captures = captures;
}

// Fill trainer
void fill_trainer(trainer *t, char *name, itinerary *visits){
    t->name = strdup(name);
    t->visits = visits;
}


/* De-allocate the strings in a monster. */
void clear_monster(monster *m)
{
    if(m->name != NULL)
    {
        free(m->name);
        m->name = NULL;
    }

    if(m->element != NULL)
    {
        free(m->element);
        m->element = NULL;
    }

    m->population = 0;
    m->id = 0;
}

/* Dispose of an array of pointers to monster */
void dispose_monster_pointer_array(monster **m, int nmonsters)
{
    for(int i = 0; i < nmonsters; i++)
    {
        clear_monster(m[i]);
        free(m[i]);
        m[i] = NULL;
    }
    free(m);
    m = NULL;
}

/* De-allocate attributes in a region. */
void clear_region(region *r)
{
    if(r->name != NULL)
    {
        free(r->name);
        r->name = NULL;
    }
    
    dispose_monster_pointer_array(r->monsters, r->nmonsters);

    r->nmonsters = 0;
    r->total_population = 0;
}

/* Dispose of an array of pointers to region */
void dispose_region_pointer_array(region **r, int nregions)
{
    for(int i = 0; i < nregions; i++)
    {
        clear_region(r[i]);
        free(r[i]);
        r[i] = NULL;
    }
    free(r);
    r = NULL;
}

/* De-allocate attributes in an itinerary. */
void clear_itinerary(itinerary *it)
{
    dispose_region_pointer_array(it->regions, it->nregions);
    it->nregions = 0;
    it->captures = 0;
}

/* De-allocate attributes in a trainer. */
void clear_trainer(trainer *t)
{
    if(t->name != NULL)
    {
        free(t->name);
        t->name = NULL;
    }

    clear_itinerary(t->visits);
}

/* Allocate an array of  monsters. */
monster *new_monster_array(int nmonsters)
{
    monster *m = calloc(nmonsters, sizeof(monster));
    return m;
}

/* Allocate an array of regions. */
region *new_region_array(int nregions)
{
    region *r = calloc(nregions, sizeof(region));
    return r;
}

/* Allocate an array of trainers. */
trainer *new_trainer_array(int ntrainers)
{
    trainer *t = calloc(ntrainers, sizeof(trainer));
    return t;
}

/* Dispose of a monster array. */
void dispose_monster_array(monster *m, int nmonsters)
{
    for(int i = 0; i < nmonsters; i++)
    {
        clear_monster(m + i);
    }
    free(m);
}

/* Dispose of a array region array */
void dispose_region_array(region *r, int nregions)
{
    for(int i = 0; i < nregions; i++)
    {
        clear_region(r + i);
    }
    free(r);
}

/* Dispose of a trainer array. */
void dispose_trainer_array(trainer *t, int ntrainers)
{
    for(int i = 0; i < ntrainers; i++)
    {
        clear_trainer(t + i);
    }
    free(t);
}

int get_number_from_line(FILE *ifp)
{
    char s[128];
    int num;

    // Get the first line of the file.
    fgets(s, 127, ifp);
    sscanf(s, "%d", &num);
    return num;
}


/* Remove carriage return and/or line feed characters from a string. */
void remove_crlf(char *s)
{
    char *t = s + strlen(s);

    // t begins at the null sentinel at the end of s.

    t--;

    /* t is now at the last character of s - unless s didn't contain any characters, in which
       case, t is now *BEFORE* s.  We have to keep checking for that. */

    /* We repeat until EITHER t slides to the left of s, OR we find a character that is not a
       line feed (\n) or a carriage return (\r). */

    while ((t >= s) && (*t == '\n' || *t == '\r'))
    {
        *t = '\0'; // Clobber the character t is pointing at.
        t--;      // Decrement t.
    }
}


/* Get the next line from an input file that isn't blank, and leave it in s.  Will clobber
   s no matter what happens.  Will crash if there isn't a next blank line. */
void get_next_nonblank_line(FILE *ifp, char *s, int max_length)
{
    s[0] = '\0';

    while (s[0] == '\0')
    {
        fgets(s, max_length, ifp);
        remove_crlf(s);
    }
}


/* Read a monster from our file. */
void read_monster(FILE *ifp, int id, monster *m)
{
    char name[128];
    char element[128];
    int population;

    /* Each monster has a name, a type, a region and a commonality. */

    get_next_nonblank_line(ifp, name, 127);
    get_next_nonblank_line(ifp, element, 127);
    population = get_number_from_line(ifp);
    fill_monster(m, id, name, element, population);
}

/* Read a region from our file. */
void read_regions(FILE *ifp, region *r, monster *m, int n)
{
    char name[128];
    int nmonsters = 0;
    int total_population = 0;
    get_next_nonblank_line(ifp, name, 127);
    nmonsters = get_number_from_line(ifp);
    monster **monsters = (monster **)calloc(nmonsters, sizeof(monster *));
    char s[128];
    for(int i=0; i < nmonsters; i++){
        scanf("%s", &s);
        for(int j=0; j<n; j++){
            if(strcmp(s, m[j].name)==0){
                monsters[i] = &m[j];
                total_population += m[j].population;
            }
        }
    }
    fill_region(r, name, nmonsters, total_population, monsters);
}

/* Read a itinerary from our file. */
void read_itinerary(FILE *ifp, itinerary *it, region *r, int n){
    int nregions = 0;
    int captures = 0;
    captures = get_number_from_line(ifp);
    nregions = get_number_from_line(ifp);
    region **regions = (region **)calloc(nregions, sizeof(region *));
    char s[128];
    for(int i=0; i < nregions; i++){
        scanf("%s", &s);
        for(int j=0; j<n; j++){
            if(strcmp(s, r[j].name)==0){
                regions[i] = &r[j];
            }
        }
    }
    fill_itinerary(it, nregions, regions, captures);  
}

/* Read a trainer from our file. */
void read_trainers(FILE *ifp, trainer *t, region *r, int n){
    char *name;
    itinerary *it;

    get_next_nonblank_line(ifp, name, 127);
    read_itinerary(ifp, it, r, n);
    fill_trainer(t, name, it);
}

/* Main function. */
int main(void)
{   
    //detect memory leak
    atexit(report_mem_leak);

    FILE *ifp;
    FILE *ofp;

    int nmonsters;
    int nregions;
    int ntrainers;
    int i;
    monster *monsters;
    region *regions;
    trainer *trainers;

    /* File output is easy: all you do is open a file, and use fprintf instead of printf.

       Open our input.txt for (r)ead and our output.txt for (w)rite.  Note that this will clobber 
       anything that's already in output.txt.  If we wanted to keep what was already there and 
       start writing at the end of it, we'd open for (a)ppend instead. */

    ifp = fopen("input.txt", "r");
    ofp = fopen("output.txt", "w");

    /* Get our number of monsters. */

    nmonsters = get_number_from_line(ifp);
    monsters = new_monster_array(nmonsters);

    for (i = 0; i < nmonsters; i++)
    {
        read_monster(ifp, i, monsters + i);
    }

    nregions = get_number_from_line(ifp);
    regions = new_region_array(nregions);

    for(i=0; i < nregions; i++){
        read_regions(ifp, regions + i, monsters, nmonsters);
    }

    ntrainers = get_number_from_line(ifp);
    trainers = new_trainer_array(ntrainers);

    for(i=0; i<ntrainers; i++){
        read_trainers(ifp, trainers + i, regions, nregions);
    }

    for(i=0; i<ntrainers; i++){
        trainer t = trainers[i];
        fprintf(ofp, "%s\n", t.name);                            // 1) trainer's name
        itinerary *it = t.visits;
        int nreg = it->nregions;
        int captures = it->captures;
        region **r_array = it->regions;

        for(int j=0; j < nreg; j++){
            region r = *r_array[i];
            fprintf(ofp, "%s\n", r.name);                        // 2) region's name
            int total = r.total_population;
            int nmons = r.nmonsters;
            monster **m_array = r.monsters;

            for(int k=0; k< nmons; k++){
                monster m = *m_array[i];
                float ratio = (m.population)/total;
                int kills = round(ratio)*captures;
                if(kills !=0)
                    fprintf(ofp, "%d %s\n", kills, m.name);     // 3) capture and monster name
            }
        }
        fprintf(ofp, "\n");                                     // 4) new line after each trainer        
    }

    dispose_monster_array(monsters, nmonsters);
    dispose_region_array(regions, nregions);
    dispose_trainer_array(trainers, ntrainers);

    fclose(ifp);
    fclose(ofp);

    return 0;
}