/******************************************
* Author:   Maya Petter                   *
* Reviewer: Adi Peretz                    *
*******************************************/

#include <stddef.h> /* size_t                */
#include <stdio.h>  /* puts, printf, sprintf */
#include <stdlib.h> /* malloc, free          */
#include <string.h> /* strcpy, strcat        */

/*************** Typdef *******************************************************/
typedef struct Metadata Metadata_t;
typedef struct Object Object_t;
typedef struct Animal Animal_t;
typedef struct Cat Cat_t;
typedef struct Dog Dog_t;
typedef struct Legendary Legen_t;

typedef void *(*v_func_t)(void *);

/*************** struct declaretions ******************************************/
struct Object 
{
	Metadata_t *metadata;	
};

struct Metadata 
{
	char *name;
	size_t size;
	Metadata_t *super;
	v_func_t *method;
};

struct Animal 
{
	Object_t object;
	int num_legs;
	int num_masters;
	int ID;
};

struct Cat 
{
	Animal_t animal;
	int num_masters;
	char color[20];
};

struct Dog 
{
	Animal_t animal;
	int num_legs;
};

struct Legendary 
{
	Cat_t cat;
};

/*************** Globals & enums **********************************************/
enum {TO_STRING, FINALIZE, SAY_HELLO, SHOW_COUNTER, GET_MASTERS};
enum {TRUE, FALSE};

static int G_COUNTER = 0;
char G_BUFFER[50] = {0};

/*************** Forword declaretions *****************************************/

/** Object methods ***************/
static void *GC(void *this);
char *toString();

/** Animal methods ***************/
char *animalToString();
static void animalFinalize(Animal_t *this);
void animalSayHello(Animal_t *this);
void showCounter();
int getNumMasters(Animal_t *this);

/** Dog methods ******************/
char *dogToString();
static void dogFinalize(Dog_t *this);
void dogSayHello(Dog_t *this);

/** Cat methods ******************/
char *catToString();
static void catFinalize(Cat_t *this);

/** Legendary methods *************/
char *legendaryToString();
static void legendaryFinalize(Legen_t *this);
void legendarySayHello();

/*************** new and constructors *****************************************/
static void *new(Metadata_t *metadata);

/*************** Global arrays ************************************************/
v_func_t obj_methods[] = {(v_func_t)toString, (v_func_t)GC};

v_func_t animal_methods[] = { (v_func_t)animalToString, 
							  (v_func_t)animalFinalize, 
							  (v_func_t)animalSayHello, 
							  (v_func_t)showCounter, 
							  (v_func_t)getNumMasters
							 };

v_func_t dog_methods[] = { (v_func_t)dogToString,
						   (v_func_t)dogFinalize,
						   (v_func_t)dogSayHello,
						   (v_func_t)showCounter, 
						   (v_func_t)getNumMasters
						  };

v_func_t cat_methods[] = { (v_func_t)catToString,
						   (v_func_t)catFinalize,
						   (v_func_t)animalSayHello, 
						   (v_func_t)showCounter, 
						   (v_func_t)getNumMasters
						  };

v_func_t legendary_methods[] = { (v_func_t)legendaryToString,
								 (v_func_t)legendaryFinalize,
								 (v_func_t)legendarySayHello,
								 (v_func_t)showCounter, 
								 (v_func_t)getNumMasters	
							    };

/*************** Type metadata ************************************************/
Metadata_t obj_meta = {"object", sizeof(Object_t), NULL, obj_methods};
Metadata_t animal_meta = {"animal", sizeof(Animal_t), &obj_meta, animal_methods};
Metadata_t dog_meta = {"dog", sizeof(Dog_t), &animal_meta, dog_methods};
Metadata_t cat_meta = {"cat", sizeof(Cat_t), &animal_meta, cat_methods};
Metadata_t legendary_meta = {"legendary", sizeof(Legen_t), &cat_meta, legendary_methods};

/*************** Object methods ***********************************************/
static void *GC(void *this)
{
	free(this); 
	this = NULL;
	
	return NULL;
}

char *toString(Object_t *this)
{
	return this->metadata->name;
}

/*************** Animal methods ***********************************************/
void animalSayHello(Animal_t *this)
{
	printf("Animal Hello!\nI have %d legs\n", this->num_legs);
}

void showCounter()
{
	printf("counter %d\n", G_COUNTER);
}

int getNumMasters(Animal_t *this)
{
	return this->num_masters;
}

static void animalFinalize(Animal_t *this)
{
	if (NULL == this)
	{
		puts("NullPointerException\n");
		return; 
	}
	
	printf("finalize Animal with ID: %d\n", this->ID);
	
	GC(this);
}

char *animalToString(Animal_t *this)
{
	sprintf(G_BUFFER, "Animal with ID: %d", this->ID);
	
	return G_BUFFER;
}

/*************** Dog methods **************************************************/
void dogSayHello(Dog_t *this)
{
	printf("Dog Hello!\nI have %d legs\n", this->num_legs);
}

static void dogFinalize(Dog_t *this)
{
	if (NULL == this)
	{
		puts("NullPointerException\n");
		return; 
	}
	
	printf("finalize dog with ID: %d\n", this->animal.ID);
	animalFinalize((Animal_t *)this);
}

char *dogToString(Dog_t *this)
{
	sprintf(G_BUFFER, "Dog with ID: %d", this->animal.ID);
	
	return G_BUFFER;
}	

/*************** Cat methods **************************************************/
static void catFinalize(Cat_t *this)
{
	if (NULL == this)
	{
		puts("NullPointerException\n");
		return; 
	}
	
	printf("finalize cat with ID: %d\n", this->animal.ID);
	animalFinalize((Animal_t *)this);
	
}

char *catToString(Cat_t *this)
{
	sprintf(G_BUFFER, "Cat with ID: %d", this->animal.ID);
	
	return G_BUFFER;
}

/*************** Legendary methods ********************************************/
void legendarySayHello()
{
	puts("Legendary Hello!\n");
}

static void legendaryFinalize(Legen_t *this)
{
	if (NULL == this)
	{
		puts("NullPointerException\n");
		return; 
	}
	
	printf("finalize LegendaryAnimal with ID: %d\n", this->cat.animal.ID);
	catFinalize((Cat_t *)this);
}

char *legendaryToString(Legen_t *this)
{
	sprintf(G_BUFFER, "LegendaryAnimal with ID: %d", this->cat.animal.ID);
	
	return G_BUFFER;
}

/*************** static initializers ******************************************/
void StaticInitAnimal()
{
	static int animal_alive = FALSE;
	
	if (FALSE == animal_alive)
	{
		puts("Static block Animal 1");
		puts("Static block Animal 2");
		animal_alive = TRUE;
	}
}

void StaticInitDog()
{
	static int dog_alive = FALSE;
	
	if (FALSE == dog_alive)
	{
		StaticInitAnimal();
		puts("Static block Dog");
		dog_alive = TRUE;
	}
}

void StaticInitCat()
{
	static int cat_alive = FALSE;
	
	if (FALSE == cat_alive)
	{
		StaticInitAnimal();
		puts("Static block Cat");
		cat_alive = TRUE;
	}
}

void StaticInitLegendary()
{
	static int legen_alive = FALSE;
	
	if (FALSE == legen_alive)
	{
		StaticInitCat();
		puts("Static block Legendary Animal");
		legen_alive = TRUE;
	}
}

/*************** new and constructors *****************************************/
static void *new(Metadata_t *metadata)
{
	Object_t *this = (Object_t *)malloc(metadata->size);
	this->metadata = metadata;
	
	return this;
}

static void AnimalCotr(Animal_t *this) 
{
    puts("Instance initialization block Animal");
	puts("Animal Cotr");

	this->ID = ++G_COUNTER;
	this->num_legs = 5;
	this->num_masters = 1;
	this->object.metadata->method[SAY_HELLO](this);
	this->object.metadata->method[SHOW_COUNTER](this);
	
	printf("%s\n", (char *)this->object.metadata->method[TO_STRING](this));
	printf("%s\n", (char *)animal_meta.super->method[TO_STRING](this));	
}

static void AnimalCotrInt(Animal_t *this, int num_masters)
{
	puts("Instance initialization block Animal");
	puts("Animal Cotr int");
	
	this->ID = ++G_COUNTER;
	this->num_legs = 5;
	this->num_masters = num_masters;
}

static void DogCotr(Dog_t *this) 
{
	AnimalCotrInt(&this->animal, 2); 
    puts("Instance initialization block Dog");
	this->num_legs = 4;
	puts("Dog Ctor");
}

static void CatColorCotr(Cat_t *this, char *color)
{
	AnimalCotr(&this->animal);
	strcpy(this->color, color);
	printf("Cat Ctor with color: %s\n" ,(char *)this->color);
}

static void CatCotr(Cat_t *this)
{
	CatColorCotr(this, "black");
	this->num_masters = 2;
	puts("Cat Ctor"); 
}

static void LegendaryAnimalCtor(Legen_t *this)
{
    CatCotr(&this->cat);
    puts("Legendary Ctor");
}

void *new_Animal(Metadata_t *metadata)
{
	Animal_t *animal = NULL;
	StaticInitAnimal();
	animal = new(metadata);
	AnimalCotr(animal);
	
	return animal;
}

void *new_Dog(Metadata_t *metadata)
{
	Dog_t *dog = NULL;
	StaticInitDog();
	dog = new(metadata);
    DogCotr(dog);
    
    return dog;
}

void *new_Cat(Metadata_t *metadata)
{
	Cat_t *cat = NULL;
	StaticInitCat();
	cat = new(metadata);
    CatCotr(cat);
    
    return cat;
}

void *new_CatColor(Metadata_t *metadata, char *color)
{
	Cat_t *cat = NULL;
	StaticInitCat();
	cat = new(metadata);
    CatColorCotr(cat, color);
    
    return cat;
}

void *new_Legendary(Metadata_t *metadata)
{
	Legen_t *la = NULL;
	StaticInitLegendary();
	la = new(metadata);
    LegendaryAnimalCtor(la);
    
    return la;
}

/*************** main Java2c **************************************************/
void foo(Animal_t *a)
{
	printf("%s\n", (char *)a->object.metadata->method[TO_STRING](a));
}

int main()
{
	Animal_t *animal = new_Animal(&animal_meta);
    Dog_t *dog = new_Dog(&dog_meta);
    Cat_t *cat = new_Cat(&cat_meta);
	Legen_t *la = new_Legendary(&legendary_meta);

	animal->object.metadata->method[SHOW_COUNTER](animal);
	
	printf("%d\n", animal->ID);
	printf("%d\n", ((Animal_t *)dog)->ID);
	printf("%d\n", ((Animal_t *)cat)->ID);
	printf("%d\n", ((Animal_t *)la)->ID);
	
	/** Animal array - new animals **/
	{
		int i = 0;
		
		Dog_t *dog2 = new_Dog(&dog_meta);
		Cat_t *cat2 = new_Cat(&cat_meta);
		Cat_t *white_cat = new_CatColor(&cat_meta, "white");
		Legen_t *legendary = new_Legendary(&legendary_meta);
		Animal_t *animal2 = new_Animal(&animal_meta);
		
		Animal_t *animal_arr[5] = {NULL};
		
		animal_arr[0] = (Animal_t *)dog2;
		animal_arr[1] = (Animal_t *)cat2;
		animal_arr[2] = (Animal_t *)white_cat;
		animal_arr[3] = (Animal_t *)legendary;
		animal_arr[4] = animal2; 

		for (i = 0; 5 > i; ++i)
		{
			animal_arr[i]->object.metadata->method[SAY_HELLO](animal_arr[i]);
			printf("%d\n", animal_arr[i]->object.metadata->method[GET_MASTERS](animal_arr[i]));
		}
		
		for (i = 0; 5 > i; ++i)
		{
			foo(animal_arr[i]);
		}
		
		for (i = 4; 0 <= i; --i)
		{
			animal_arr[i]->object.metadata->method[FINALIZE](animal_arr[i]);
		}
	}
		
		((Animal_t *)la)->object.metadata->method[FINALIZE](la);
		((Animal_t *)cat)->object.metadata->method[FINALIZE](cat);
		((Animal_t *)dog)->object.metadata->method[FINALIZE](dog);
		animal->object.metadata->method[FINALIZE](animal);
		
	return 0;
}
