#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define MAX_SONGS_PER_USER   10
#define MAX_USERS    		  5
#define MAX_ARTISTS		    100


#define NUM_TEST_USERS 		7


// This struct represents a song in the system

typedef struct {
	char     *name;
	short int downloads;
} Artist;

typedef struct {
	char      *title;
	char      *artist;
	short int  duration;
} Song;

// This struct represents a user in the system
typedef struct {
	char       *name;
	char        online; 				   // 1 = YES, 0 = NO
	Song       *songs[MAX_SONGS_PER_USER]; // songs this user has
	short int   numSongs;  				   // number of songs this user has
} User;

// This structure represents the Tune Share system
typedef struct {
	User  		*users[MAX_USERS];     // An array of all registered users
	Artist      *artists[MAX_ARTISTS]; //An array of all artists
	short int	 numUsers;
	short int    numArtists;
} TuneShare;


//Forward declaration
int registerUser(TuneShare *t, char *name);
int addSong(User *u, char *title, char *artist, short int duration);
User * userWithName(TuneShare *t, char *name);
void logon(TuneShare *t, char *name);
void logoff(TuneShare *t, char *name);
User ** onlineUsers(TuneShare *t, short int *numOnLine);
Song ** allAvailableSongs(TuneShare *t, short int *numSongs);
void displayStats(TuneShare *t);
void displayAvailableSongsByArtist(TuneShare *t, char *artist);
Song * getSong(TuneShare *t, char *title, char *ownerName);
void downloadSong(TuneShare *t, char *downloadToName, char *title, char *downloadFromName);
void displayRoyalties(TuneShare *t);
void addArtist(TuneShare *t, char *name);
void shutDown(TuneShare *t);



/* ADD ALL YOUR FUNCTIONS HERE */
int registerUser(TuneShare *t, char *name){ //Should be done
	for(int i=0; i< MAX_USERS; i++){

		if (t->numUsers >= MAX_USERS){
			return 0;
		}
		if(strcmp(t->users[i]->name, name) == 0){
			return 0;
		}
		t->users[t->numUsers] = (User *) malloc(sizeof(User));
		t->users[t->numUsers]->name = name;
		t->users[t->numUsers]->online = 0;
		t->users[t->numUsers]->numSongs = 0;
		t->numUsers++;
		return 1;

		printf("%s\n", t->users[i]->name);

	}
}

int addSong(User *u, char *title, char *artist, short int duration){ //Should be done
	if(u->numSongs >= MAX_SONGS_PER_USER){
		return 0;
	}
	
	u->songs[u->numSongs] = (Song *) malloc(sizeof(Song));
	u->songs[u->numSongs]->title = title;
	u->songs[u->numSongs]->artist = artist;
	u->songs[u->numSongs]->duration = duration;
	u->numSongs++;

	return 1;
}

User * userWithName(TuneShare *t, char *name){ //Should be done
	for(int i=0; i<t->numUsers; i++){
		if(strcmp(t->users[i]->name, name) == 0){
			return t->users[i];
		}
	}
	return NULL;
}

void logon(TuneShare *t, char *name){ //Should be done
	User *user = userWithName(t, name);
	if(user != NULL){
		user->online = 1;
		return;
	}
	return;
}

void logoff(TuneShare *t, char *name){ //Should be done
	User *user = userWithName(t,name);
	if(user != NULL){
		user->online = 0;
		return;
	}
	return;
}

User ** onlineUsers(TuneShare *t, short int *numOnLine){ //Should be done
	int counter = 0;
	for(int i=0; i<t->numUsers; i++){
		if(t->users[i]->online == 1){
			counter++;
		}
	}

	*numOnLine = counter;
	User **onlineUsers = (User **) malloc(sizeof(User *) * counter);

	counter = 0;
	for(int i=0; i<t->numUsers; i++){
		if(t->users[i]->online == 1){
			onlineUsers[counter] = (User *) malloc(sizeof(User));
			onlineUsers[counter] = t->users[i];
			counter++;
		}
	}
	return onlineUsers;
}

Song ** allAvailableSongs(TuneShare *t, short int *numSongs){ //Should be done
	int counter = 0;
	for(int i=0; i<t->numUsers; i++){
		if(t->users[i]->online == 1){
			counter += t->users[i]->numSongs;
		}
	}

	*numSongs = counter;

	Song **availableSongs = (Song **) malloc(sizeof(Song *) * counter);

	counter = 0;
	for(int i=0; i<t->numUsers; i++){
		if(t->users[i]->online == 1){
			for(int j=0; j< t->users[i]->numSongs; j++){
				availableSongs[counter] = (Song *) malloc(sizeof(Song));
				if(availableSongs[counter] == NULL){
					printf("Memory allocation error\n");
				}
				availableSongs[counter] = t->users[i]->songs[j];
				counter++;
			}
		}
	}
	return availableSongs;
}

void displayStats(TuneShare *t){  //Should be done
	short int numOnline;
	short int numSongs;
	User **users = onlineUsers(t, &numOnline);
	Song **availableSongs = allAvailableSongs(t, &numSongs);

	printf("\n");
	printf("Tune Share Center (%d registered users)\n\n", t->numUsers);
	printf("\t%d Online Users:\n", numOnline);
	if(numOnline == 0){
		printf("\t\tNONE");
		printf("\n");
	}
	else{
		for(int i=0; i<numOnline; i++){
			
			printf("\t\t%s with %d songs as follows:\n",(*(users+i))->name, (*(users+i))->numSongs);
			
			for(int j=0; j < (*(users+i))->numSongs; j++){
				printf("\t\t\t\"%s\" by %s\n",(*(users+i))->songs[j]->title, (*(users+i))->songs[j]->artist);
			}
		}
		printf("\n");
	}
	
	printf("\n");
	printf("\t%d Available Songs:\n", numSongs);
	if(numSongs == 0){
		printf("\t\tNONE\n");
	}
	else{
		for(int i=0; i<numSongs; i++){
			printf("\t\t\"%s\" by %s (%d:%02d)\n",(*(availableSongs+i))->title, (*(availableSongs+i))->artist, (*(availableSongs+i))->duration/60, (*(availableSongs+i))->duration%60);
		}
		printf("\n");
	}
}

void displayAvailableSongsByArtist(TuneShare *t, char *artist){  //Should be done
	int printed = 0;
	short int numSongs;
	Song **availableSongs = allAvailableSongs(t, &numSongs);

	for(int i=0; i<numSongs; i++){
		if(strcmp((*(availableSongs+i))->artist, artist) == 0){
			printf("\t\"%s\" by %s (%d:%02d)\n", (*(availableSongs+i))->title, (*(availableSongs+i))->artist, (*(availableSongs+i))->duration/60, (*(availableSongs+i))->duration%60);
			printed = 1;
		}
	}
	if(printed == 0){
		printf("\tNONE\n");
	}
}

Song * getSong(TuneShare *t, char *title, char *ownerName){ //Should be done
	User *user = userWithName(t, ownerName);
	if (user == NULL || user->online == 0){
		return NULL;
	}
	for(int i=0; i<user->numSongs; i++){
		if(strcmp(user->songs[i]->title, title) == 0){
			return user->songs[i];
		}
	}
	return NULL;
}

void downloadSong(TuneShare *t, char *downloadToName, char *title, char *downloadFromName){ //Should be done
	User *userA = userWithName(t, downloadToName);
	User *userB = userWithName(t, downloadFromName);
	Song *song = getSong(t, title, downloadFromName);
	
	if(userA == NULL || userB == NULL || song == NULL || userA->online == 0 || userB->online == 0){
		return;
	}

	addSong(userA, song->title, song->artist, song->duration);
	addArtist(t, song->artist);
}

void displayRoyalties(TuneShare *t){ //Should be done
	for(int i=0; i<t->numArtists; i++){
		printf("$\t%d.%d %s\n", t->artists[i]->downloads*25/100, t->artists[i]->downloads*25%100, t->artists[i]->name);
	}
}

void addArtist(TuneShare *t, char *name){ //Should be done
	if(t->numArtists == 0){
		t->numArtists++;
		t->artists[0] = (Artist *) malloc(sizeof(Artist));
		t->artists[0]->name = name;
		t->artists[0]->downloads = 1;
		return;
	}

	for(int i=0; i<t->numArtists; i++){
		if(strcmp(t->artists[i]->name, name) == 0){
			t->artists[i]->downloads++;
			return;
		}
	}

	t->artists[t->numArtists] = (Artist *) malloc(sizeof(Artist));
	t->artists[t->numArtists]->name = name;
	t->artists[t->numArtists]->downloads = 1;
	t->numArtists++;
	return;
}

void shutDown(TuneShare *t){
	for(int i=0; i<t->numUsers; i++){
		t->users[i]->online = 0;
	}


}


// DO NOT MODIFY THE MAIN FUNCTION
int main() {
	// This is test data
	static char *TITLES[] = {"Back 2 Life", "Loneliness For Love", "Desire 126", "Perfect", "In My Head", 
							 "Old Skool Love", "Let's Go", "No Place", "We Gotta Go", "How You Get the Girl", 
							 "Catch", "Here Tonight", "Details", "Dangerous", "Brighter Than the Sun",
				  			 "Photograph", "Thinking Out Loud", "If Heaven Were to Fall", "I Just Wanna Be With You", 
				  			 "Song Sung Blue", "Outta Style", "Why", };
				  
	static char *ARTISTS[] = {"E-Type", "Lovely the Band", "Hollerado", "Ed Sheeran", "Ryland James", 
							  "Divine Brown", "Group 1 Crew", "Backstreet Boys", "E-Type", "Taylor Swift", 
							  "Brett Young", "Brett Young", "Billy Currington", "Kardinal Offichall", 
							  "Colbie Caillat", "Ed Sheeran", "Ed Sheeran", "E-Type", "E-Type", "Neil Diamond", 
				   			  "Aaron Watson", "Frankie Avalon", };
				   
	static int   DURATIONS[] = {217, 237, 187, 263, 205, 204, 256, 179, 213, 247, 196, 
								216, 201, 251, 231, 202, 281, 223, 230, 185, 222, 161};


	static char *TEST_USER_NAMES[NUM_TEST_USERS] = {"Disco Stew", "Peter Punk", "Country Candy", "Ronnie Rocker", 
													"Sleeping Sam", "Disco Stew", "Mellow Marvin"};

	static int  LIST_SIZES[NUM_TEST_USERS] = {7, 9, 9, 5, 1, 0, 0};
	static int  SONGNUMS[NUM_TEST_USERS][MAX_SONGS_PER_USER] = {
								   {1, 2, 4, 5, 12, 15, 21}, {0, 1, 3, 8, 9, 13, 14, 17, 20}, 
								   {6, 7, 8, 10, 11, 12, 13, 20, 21}, {0, 8, 16, 17, 18}, {19}, {}, {}};

	
	// Create the TuneShare Center
	TuneShare   tuneShareCenter;
	tuneShareCenter.numUsers = 0;
	tuneShareCenter.numArtists = 0;
	
	// Attempt to register all test users
	for (int i=0; i<NUM_TEST_USERS; i++) {
		if (!registerUser(&tuneShareCenter, TEST_USER_NAMES[i]))
			printf("Error adding User: \"%s\"\n", TEST_USER_NAMES[i]);
		else
			printf("User: \"%s\" has been registered\n", TEST_USER_NAMES[i]);
	}

	// Display some stats
	displayStats(&tuneShareCenter);

	// Log on a user
	printf("\nLogging on a user...\n");
	logon(&tuneShareCenter, "Disco Stew");
	
	// Display some stats
	displayStats(&tuneShareCenter);

	// Now add all the test songs for these test users
	for (int i=0; i<tuneShareCenter.numUsers; i++) {
		for (int j=0; j<LIST_SIZES[i]; j++) 
			addSong(tuneShareCenter.users[i], TITLES[SONGNUMS[i][j]], ARTISTS[SONGNUMS[i][j]], DURATIONS[SONGNUMS[i][j]]);
	}

	// Display some stats
	displayStats(&tuneShareCenter);

	// Display all songs by E-Type
	printf("Available Songs By E-Type: \n");
	displayAvailableSongsByArtist(&tuneShareCenter, "E-Type");

	// Now bring two more users online
	printf("\nLogging on three users...\n");
	logon(&tuneShareCenter, "Disco Stew");    // Already logged on ... does nothing
	logon(&tuneShareCenter, "Mellow Marvin"); // Won't Work
	logon(&tuneShareCenter, "Peter Punk");
	logon(&tuneShareCenter, "Country Candy");

	// Display some stats again
	displayStats(&tuneShareCenter);
	
	// Log the last two users online
	printf("\nLogging on two more users...\n");
	logon(&tuneShareCenter, "Ronnie Rocker");
	logon(&tuneShareCenter, "Sleeping Sam");

	// Display some stats again
	displayStats(&tuneShareCenter);

	// Display all songs by E-Type
	printf("Available Songs By E-Type: \n");
	displayAvailableSongsByArtist(&tuneShareCenter, "E-Type");
	
	// Log off a couple of users
	printf("\nLogging off some users...\n");
	logoff(&tuneShareCenter, "Country Candy");
	logoff(&tuneShareCenter, "Mellow Marvin"); // Won't work
	logoff(&tuneShareCenter, "Peter Punk");

	// Display some stats again
	displayStats(&tuneShareCenter);
	
	// Display all songs by E-Type
	printf("Available Songs By E-Type: \n");
	displayAvailableSongsByArtist(&tuneShareCenter, "E-Type");

	// Have Sleeping Sam download some songs from other online users
	printf("\nSleeping Sam downloading 2 songs from Disco Stew and 2 from Ronnie Rocker...\n");
	downloadSong(&tuneShareCenter, "Sleeping Sam", "Old Skool Love", "Disco Stew");
	downloadSong(&tuneShareCenter, "Sleeping Sam", "In My Head", "Disco Stew");
	downloadSong(&tuneShareCenter, "Sleeping Sam", "If Heaven Were to Fall", "Ronnie Rocker");
	downloadSong(&tuneShareCenter, "Sleeping Sam", "I Just Wanna Be With You", "Ronnie Rocker");
	downloadSong(&tuneShareCenter, "Sleeping Sam", "We Gotta Go", "Country Candy"); 
	
	// Have Ronnie Rocker download from Sleeping Sam and Disco Stew
	printf("\nRonnie Rocker downloading a songs from Sleeping Sam and a song from Disco Stew...\n");
	downloadSong(&tuneShareCenter, "Ronnie Rocker", "Song Sung Blue", "Sleeping Sam"); 
	downloadSong(&tuneShareCenter, "Ronnie Rocker", "Desire 126", "Disco Stew"); 
	
	// None of these should work
	downloadSong(&tuneShareCenter, "Sleeping Sam", "Back 2 Life", "Disco Stew");		// Won't work ... not a song of his
	downloadSong(&tuneShareCenter, "Sleeping Sam", "No Place", "Mellow Marvin"); 		// Won't work, not a user
	downloadSong(&tuneShareCenter, "Mellow Marvin", "Song Sung Blue", "Sleeping Sam"); 	// Won't work, not registered user
	downloadSong(&tuneShareCenter, "Ronnie Rocker", "Song Sung Blue", "Mellow Marvin"); // Won't work, not registered user
	downloadSong(&tuneShareCenter, "Country Candy", "Song Sung Blue", "Sleeping Sam"); 	// Won't work, not logged on
	downloadSong(&tuneShareCenter, "Sleeping Song", "Popsicles", "Ronnie Rocker"); 		// Won't work, song doesn't exist

	// Log on Country Candy and have her download a song from Ronnie
	printf("\nLogging on Country Candy...\n");
	logon(&tuneShareCenter, "Country Candy");
	downloadSong(&tuneShareCenter, "Country Candy", "Song Sung Blue", "Ronnie Rocker"); 

	// Display some stats again
	displayStats(&tuneShareCenter);

	// Log off the last few users
	printf("\nLogging off three more users...\n");
	logoff(&tuneShareCenter, "Disco Stew");
	logoff(&tuneShareCenter, "Ronnie Rocker");
	logoff(&tuneShareCenter, "Sleeping Sam");
	logoff(&tuneShareCenter, "Sleeping Sam"); // Won't Work now

	// Display some stats again
	displayStats(&tuneShareCenter);
	
	// Display all songs by E-Type
	printf("Available Songs By E-Type: \n");
	displayAvailableSongsByArtist(&tuneShareCenter, "E-Type");

	// Now display the royalties for all downloaded songs at 25 cents per song
	printf("\nHere are the royalties to be paid:\n");
	displayRoyalties(&tuneShareCenter);

	// Shut down the Tune Share center
	shutDown(&tuneShareCenter);
}
