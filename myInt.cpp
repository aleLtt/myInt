#ifndef MYINT_H
#define MYINT_H

#include <stdio.h>
#include <stdlib.h>
#include <vector>

using namespace std;

class myInt{
	public:
        myInt();    //empty constructor
        myInt(unsigned char* string, short n_char);	//constructor with initialization
        myInt(unsigned short number, unsigned int a);	//constructor with elements empty
        myInt(unsigned int number, unsigned int a);  //constructor with initialization        
        myInt(unsigned long long number, unsigned int a); //constructor with elements empty
        ~myInt();   //destructor

        //UTILITY FUNCTION
        void print_value(); //print the value field
        void set_new_value(unsigned char* arg, short n_char);   //build a new container value -> MODIFICARNE L'IDEA??
        unsigned long long get_length();    // length of *this
        
            //PER IL MOMENTO IMPLEMENTATA CHE IL MODULO PU0' ESSERE SOLO MULTIPLO DI 16
            myInt* mod_sum(myInt* num, unsigned int mod);  //modulo sum of two numbers
            myInt* self_mod_sum(myInt* num, unsigned int mod);  //modulo sum applied to *this
        
            //PER IL MOMENTO IMPLEMENTATO CHE RESTITUISCA SOLO A GRUPPI DI 16 E START= {0,16, 32, ......}
            myInt* get_block_bits(unsigned long long start, unsigned long long end);    //return block from bit 'start' (comprised) to 'end' (comprised)

        myInt* copy();  //make a copy of *this

        //CREARE UNA FUNZIONE "CONCATENATE" CHE PERMETTE DI CONCATENARE I VALORI IN INPUT IN UN UNICO ELEMENTO???

        unsigned char to_char();    //parse *this into a char variable
        unsigned short to_short();  //parse *this into a short variable
        unsigned int to_int();  //parse *this into a int variable
        unsigned long long to_longlong();   //parse *this into a long long variable

        //ALGEBRAIC OPERATORS        
		void operator += (const myInt n);
        myInt* operator + (const myInt n);  //NEED TO FIX THE ERROR ON SUM AND CHECK FOR OVERFLOW CONDITION
        void operator += (const unsigned short add);
        void operator += (const unsigned int add);
        void operator += (const unsigned long long add);

        //NEED TO ADJUST THE SIZE IF FIRST ELEMENTS GET TO 0
        void operator -= (const myInt n);   //NEED TO THROUGH AN EXCEPTION IF SUBTRACTING FOR A GREATER NUMBER
        void operator -= (const unsigned short sub);   //NEED TO THROUGH AN EXCEPTION IF SUBTRACTING FOR A GREATER NUMBER
        myInt* operator - (const myInt n);

        void operator *= (const myInt n);
        void operator *= (const unsigned short mult);
        myInt* operator * (const myInt n);

        //COMPARISON OPERATORS
        bool operator == (const myInt n);
        bool operator != (const myInt n);
        bool operator > (const myInt n);
        bool operator >= (const myInt n);
        bool operator < (const myInt n);
        bool operator <= (const myInt n);

        //SHIFT OPERATIONS
        void operator <<= (const unsigned short shift);  //left shift. addition of 0s at the end => FARE IL ROUND SHIFT E LO SHIFT NORMALE CON #BIT FISSO
        void self_round_left_shift(const unsigned short shift);  //round left shift //NON FUNZIONA SE SHIFT E' UGUALE AL NUMERO TOTALE DEI BIT, FA UN ERRORE
        void operator >>= (const unsigned short shift);  //right shift. addition of 0s at the beginning and lost of less significant bits that go out of the bound
        void self_round_right_shift(const unsigned short shift); //round rigtht shift. Need to keep in mind that the value container is fulfilled with 0s to complete the size of the element

        myInt* operator << (const unsigned short shift);
        myInt* round_left_shift(const unsigned short shift); //NON FUNZIONA SE SHIFT E' UGUALE AL NUMERO TOTALE DEI BIT, FA UN ERRORE
        myInt* operator >> (const unsigned short shift);
        myInt* round_right_shift(const unsigned short shift);

        //BINARY OPERATORS
        void operator &= (const myInt n);
        myInt* operator & (const myInt n);

        void operator |= (const myInt n);
        myInt* operator | (const myInt n);

        void operator ^= (const myInt n);
        myInt* operator ^ (const myInt n);
        
        myInt* operator ~();

    private:
        //ATTRIBUTES
		vector <unsigned short> value;	//pointer to the memory space for storing the number
		unsigned short n_elements;	//number of short int stored
		bool endian;	//define the endianess of the system. 1 if Little, 0 if Big.
		
        //UTILITY FUNCTIONS
        vector <unsigned short> get_new_value(short new_size);
        bool check_endian();
};


/**
 * @brief Empty constructor.
 */
myInt::myInt(){}

/**
 * @brief Constructor of a myInt element populated with argument passed.
 * 
 * @param arg: pointer to a null terminated string.
 * @param n_char: number of character of the string with considering the null terminator string -> in this way possible to pass the argument as sizeof()
 */

myInt::myInt(unsigned char* arg, short n_char){
    endian= check_endian();    //check the endianess

	n_elements= n_char/sizeof(short);  //compute the number of 'short int' we need to contain the given input 
	value.resize(n_elements);

    unsigned char* current= arg; //used to scan through the string

    short odd_check= 0; //used ton check for odd true characters
    if((n_char-1)%2 == 1)   odd_check=1;
    
    for(short i=0; i<n_elements; ++i){
        char* point= (char*) &(value[n_elements-1-i]); //used to access the single bytes of each element of value (each element is composed by two bytes)

        if(i==n_elements-1 && odd_check){   //if we are at the last iteration and having odd characters, then need to properly handle
            *point= 0*(1-endian) + endian*arg[(n_char-1-2*i)-1*endian];
            *(point+1)= 0*endian + (1-endian)*arg[(n_char-1-2*i)-1*endian];
        }else{
            *point= arg[(n_char-1-2*i)-(2-1*endian)];
            *(point+1)= arg[(n_char-1-2*i)-(1+1*endian)];
        }
    }
}

/**
 * @brief Constructor that builds up the myInt object.
 * 
 * @param number: input parameter
 * @param a: defines whether @param number shall be interpreted as initialization number (a=1) or as the number of 0 bits initialization (a=0).
 */

myInt::myInt(unsigned short number, unsigned int a){
    
    endian= check_endian();    //check the endianess
    
    if(a){  //number is the initialization value
        
        n_elements= 1;  //we have a short so just require one element
        value.resize(n_elements);
        value[0] += number;

    }else{  //0 initialization

        //understand how many elments are required for the desired number of bits storage
        n_elements= (number%16 > 1)?    number/16 +1 : number/16;  //integer division floor approzimate, so need to understand if we need one more.
        value.resize(n_elements);

        for(short i=0; i<n_elements; ++i)   value[i]= 0;
    }
}

/**
 * @brief Constructor that builds up the myInt object
 * 
 * @param number: input parameter
 * @param a: defines whether @param number shall be interpreted as initialization number (a=1) or as the number of 0 bits initialization (a=0).
 */

myInt::myInt(unsigned int number, unsigned int a){

    endian= check_endian();    //check the endianess

    if(a){  //number is the initialization value
        
        n_elements= 2;  //integer value is two short values
        value.resize(n_elements);

        unsigned short *p= (unsigned short *) &number;   //pointer to store the two half of the int
    
        value[0]= *(p+1*endian);
        value[1]= *(p+1*(1-endian));

    }else{  //0 initialization

        //understand how many elments are required for the desired number of bits storage
        n_elements= (number%16 > 1)?    number/16 +1 : number/16;  //integer division floor approzimate, so need to understand if we need one more.
        value.resize(n_elements);

        for(short i=0; i<n_elements; ++i)   value[i]= 0;
    }    
}

/**
 * @brief Constructor that builds up the myInt object
 * 
 * @param num_bits: number of bits we want for our object
 * @param a: defines whether @param number shall be interpreted as initialization number (a=1) or as the number of 0 bits initialization (a=0).
 */

myInt::myInt(unsigned long long number, unsigned int a){

    endian= check_endian();    //check the endianess

    if(a){  //number is the initialization value
        
        n_elements= 4;  //integer value is two short values
        value.resize(n_elements);

        unsigned short *p= (unsigned short *) &number;   //pointer to store the two half of the int

        if(endian){
            for(unsigned short i=0; i<4; ++i)   value[i]= *(p+3-i);
        }else{
            for(unsigned short i=0; i<4; ++i)   value[i]= *(p+i);
        }

    }else{  //0 initialization

        //understand how many elments are required for the desired number of bits storage
        n_elements= (number%16 > 1)?    number/16 +1 : number/16;  //integer division floor approzimate, so need to understand if we need one more.
        value.resize(n_elements);

        for(short i=0; i<n_elements; ++i)   value[i]= 0;
    }
}

/**
 * @brief Distructor.
 */

myInt::~myInt(){

    value.~vector();
    n_elements= 0;
    endian= 0;
}

/**
 * @brief Prints the value field
 */

void myInt::print_value(){

    if(n_elements == 0) printf("0x-0\n");
    else{
        printf("0x-");
        for(short i=0; i<n_elements; ++i){
            printf("%04x", value[i]);
        }
        puts("");
    }
}

/**
 * @brief Change the value field. Works exactly as the constructor
 * @param arg: pointer to a null terminated string.
 * @param n_char: number of character of the string with considering the null terminator string -> in this way possible to pass the argument as sizeof()
 */

void myInt::set_new_value(unsigned char* arg, short n_char){

    endian= check_endian();    //check the endianess

	n_elements= n_char/sizeof(short);  //compute the number of 'short int' we need to contain the given input
	vector <unsigned short> new_value(n_elements, 0);

    unsigned char* current= arg; //used to scan through the string

    short odd_check= 0; //used ton check for odd true characters
    if((n_char-1)%2 == 1)   odd_check=1;
    
    for(short i=0; i<n_elements; ++i){
        char* point= (char*) &new_value[n_elements-1-i]; //used to access the single bytes of each element of value (each element is composed by two bytes)

        if(i==n_elements-1 && odd_check){   //if we are at the last iteration and having odd characters, then need to properly handle
            *point= 0*(1-endian) + endian*arg[(n_char-1-2*i)-1*endian];
            *(point+1)= 0*endian + (1-endian)*arg[(n_char-1-2*i)-1*endian];
        }else{
            *point= arg[(n_char-1-2*i)-(2-1*endian)];
            *(point+1)= arg[(n_char-1-2*i)-(1+1*endian)];
        }
    }

    value.resize(n_elements);
    for(int i=0; i<n_elements; i++)   value[i]= new_value[i];

    new_value.~vector();
}

/**
 * @brief Return the number of bits of *this. Must keep in mind that the real object has a true length that is multiple of 16 and may be longer, because of the use of type short elements
 * 
 * @return unsigned long long
 */

unsigned long long myInt::get_length(){

    unsigned short true_this=0;   //number of zero elements et the beginning. As it is possible that there are few 0 elements in front derived from operations. 
    for(true_this; value[true_this]==0 && true_this < n_elements; ++true_this);

    unsigned long long length= (n_elements-true_this)*16;

    return length;
}

/**
 * @brief 
 * 
 * @param start: index of the starting bit of the desired block. Start from index 0.
 * @param end: index of the end bit of the desired block
 * 
 * @note The indexing of the bits starts from the right
 *  
 * @return myInt* populated with the desired block. If @param start > @param end, NULL is returned. IF *this.length < end, NULL is returned.
 */

myInt* myInt::get_block_bits(unsigned long long start, unsigned long long end){

    if(start > end) return NULL;
    if(end > n_elements*16-1)   return NULL;
    
    unsigned int needed_elements= ((end-start+1)%16 > 0)?   (end-start+1)/16 +1 : (end-start+1)/16; //numer of elements needed to build the desired block
    unsigned int starting_block= start/16;  //index of the starting block
    unsigned int copy_bits= end-start+1;    //number of bits that remains to copy

    myInt* block= new myInt((unsigned int) (needed_elements*16), 0); //0 populated
    myInt* temp= *this >> start;    //create a new object where I removed all the fist bits that are not needed

    unsigned int i=0;
    for(i; i<needed_elements-1; ++i)  (*block).value[(*block).n_elements-1-i]= (*temp).value[(*temp).n_elements-1-i];

    unsigned short moment= (*temp).value[(*temp).n_elements-1-i] << 16-(copy_bits-16*i);
    moment >>= 16-(copy_bits-16*i);
    (*block).value[(*block).n_elements-1-i]= moment;

    temp->~myInt(); 

    return block;
}

/**
 * @brief Makes the modulo sum of *this and @param num
 * 
 * @param num: number to sum
 * @param mod: module of the operation
 * 
 * @return myInt* 
 */

myInt* myInt::mod_sum(myInt* num, unsigned int mod){

    unsigned long long num_elem= mod/16;    //understand how many elements needed
    short overflow= 0;	//variable to check if there was overflow when summing element by element. 1 if overflow occured

    myInt* result= new myInt((unsigned long long) num_elem*16, 0); //create the container result 0 populated

    for(unsigned long long i=0; i<num_elem; ++i){

        unsigned short temp= value[n_elements-1-i];  //need to save for future check of overflow
        (*result).value[(*result).n_elements-1-i]= value[n_elements-1-i] + (*num).value[(*num).n_elements-1-i] + 1*overflow;   //if there was an overflow, then we need to add also a 1     
        overflow= (((*result).value[(*result).n_elements-1-i]<temp) || ((*result).value[(*result).n_elements-1-i]==temp && (*num).value[(*num).n_elements-1-i]!=0))?  1:0;    //check for overflow
    }

    return result;
}

/**
 * @brief Makes the modulo sum of *this and @param num, modifying *this
 * 
 * @param num: number to sum
 * @param mod: module of the operation
 * 
 * @return myInt* 
 */

myInt* myInt::self_mod_sum(myInt* num, unsigned int mod){

    unsigned long long num_elem= mod/16;    //understand how many elements needed
    short overflow= 0;	//variable to check if there was overflow when summing element by element. 1 if overflow occured

    vector <unsigned short> new_value(num_elem, 0);

    for(unsigned long long i=0; i<num_elem; ++i){

        unsigned short temp= value[n_elements-1-i];  //need to save for future check of overflow
        new_value[num_elem-1-i]= value[n_elements-1-i] + (*num).value[(*num).n_elements-1-i] + 1*overflow;   //if there was an overflow, then we need to add also a 1     
        overflow= ((new_value[num_elem-1-i]<temp) || (new_value[num_elem-1-i]==temp && (*num).value[(*num).n_elements-1-i]!=0))?  1:0;    //check for overflow
    }
    
    n_elements= num_elem;
    value.resize(n_elements);
    for(int i=0; i<n_elements; i++)   value[i]= new_value[i];    

    new_value.~vector();
}

/**
 * @brief Return a new object that is a copy of *this
 * 
 * @return myInt* 
 */

myInt* myInt::copy(){

    myInt* result= new myInt((unsigned short) (n_elements*16), 0);

    for(int i=0; i<n_elements; ++i){
        (*result).value[i]= value[i];
    }

    return result;
}

/**
 * @brief Parse *this into a char variable
 * 
 * @note: the operation is done starting from the less significant bits of the object
 * 
 * @return unsigned char 
 */

unsigned char myInt::to_char(){

    unsigned char* p= (unsigned char*) &value[n_elements-1];
    unsigned char a= *p;

    return a;
}

/**
 * @brief Parse *this into a short variable
 * 
 * @note: the operation is done starting from the less significant bits of the object
 * 
 * @return unsigned short 
 */

unsigned short myInt::to_short(){

    return value[n_elements-1];
}

/**
 * @brief Parse *this into a int variable
 * 
 * @note: the operation is done starting from the less significant bits of the object
 * 
 * @return unsigned int 
 */

unsigned int myInt::to_int(){

    unsigned int result= 0;
    unsigned short *p= (unsigned short*) &result;

    for(int i=0; i<2 && i<n_elements; ++i){
        *(p+i)= value[n_elements-(i+1)];
    }
}

/**
 * @brief Parse *this into a long long variable
 * 
 * @note: the operation is done starting from the less significant bits of the object
 * 
 * @return unsigned long long 
 */

unsigned long long myInt::to_longlong(){

    unsigned long long result= 0;
    unsigned short *p= (unsigned short*) &result;

    for(int i=0; i<4 && i<n_elements; ++i){
        *(p+i)= value[n_elements-(i+1)];
    }

    return result;
}


//------------------------- ALGEBRIC OPERATIONS ------------------------------

/**
 * @brief Overwrite of the += operator with a myInt object. 
 */

void myInt::operator += (const myInt n){

	short overflow= 0;	//variable to check if there was overflow when summing element by element. 1 if overflow occured
    short min_n_elements= (n_elements <= n.n_elements)?   n_elements : n.n_elements;    //in general the two objects can have different sizes. Need to porperly handle
    
    if(n_elements < n.n_elements){  //in case we are adding a value with more elements, we need to correctly fit the size of *this

        vector <unsigned short> new_value= get_new_value(n.n_elements);  //obtain a new value field populated as the previous one        
        
        for(int i= n_elements; i<n.n_elements; ++i){
            new_value[n.n_elements-1-i]= n.value[n.n_elements-1-i]; //copy the values of n.value that exceeds the boundary of *this
        }

        n_elements= n.n_elements;   //update the number of elements
        value.resize(n_elements);
        for(int i=0; i<n_elements; i++)   value[i]= new_value[i];    

        new_value.~vector();
    }
    
    short i=0;
	for(i; i<min_n_elements; ++i){
        unsigned short temp= value[n_elements-1-i];  //need to save for future check of overflow
        value[n_elements-1-i]+= n.value[n.n_elements-1-i]+1*overflow;   //if there was an overflow, then we need to add also a 1
        overflow= ((value[n_elements-1-i]<temp) || ((value[n_elements-1-i]==temp) && n.value[n.n_elements-1-i]!=0))?  1:0;    //check for overflow
    }

    //if *this had more elements that n but there is a pendand overflow, need to add it
    for(i; overflow && i<n_elements; ++i){  //need to sum only untill we have overflow but remain in the boundaries
        unsigned short temp= value[n_elements-1-i];  //need to save for future check of overflow
        value[n_elements-1-i]+= 1;   //if there was an overflow, then we need to add also a 1
        overflow= (value[n_elements-1-i]<=temp)?  1:0;    //check for overflow
    }

    if(overflow){   //if reached the end but with a pendant overflow, then need to increase the number of elements
        vector <unsigned short> new_value= get_new_value(n_elements+1);  //obtain a new value field populated as the previous one
        new_value[0]= 1;

        n_elements++;
        value.resize(n_elements);
        for(int i=0; i<n_elements; i++)   value[i]= new_value[i];    

        new_value.~vector();
    }
}

/**
 * @brief Overwrite of the + operator returning a myInt object.
 */

myInt* myInt::operator + (const myInt n){

    unsigned short new_n_elements= 0;

    //first we need need to understand the value of n_elements for the result
    if((n_elements == n.n_elements) && (((unsigned short)(value[0]+n.value[0]) < value[0]) || (value[0]+n.value[0] == 0xfffe))){

        //if they have the same number of elements, check the sum of the first element to understand how likely it is for overflow
        //if enter here means we have an overflow or it is likely we have one
        new_n_elements= (n_elements < n.n_elements)?    (n.n_elements+1) : (n_elements+1);

    }else   new_n_elements= (n_elements < n.n_elements)?    n.n_elements : n_elements;  //probably not have overflow

    myInt* result= new myInt((unsigned short) (new_n_elements*16), 0);

    if(n_elements < n.n_elements){ //copy the values of n.value that exceeds the boundary of *this
        for(int i= n_elements; i<n.n_elements; ++i){
            (*result).value[(*result).n_elements-1-i]= n.value[n.n_elements-1-i]; 
        }
    }

    short overflow= 0;	//variable to check if there was overflow when summing element by element. 1 if overflow occured
    short min_n_elements= (n_elements <= n.n_elements)?   n_elements : n.n_elements;    //in general the two objects can have different sizes. Need to porperly handle
    
    short i=0;
	for(i; i<min_n_elements; ++i){

        unsigned short temp= value[n_elements-1-i];  //need to save for future check of overflow
        (*result).value[(*result).n_elements-1-i]= value[n_elements-1-i] + n.value[n.n_elements-1-i] + 1*overflow;   //if there was an overflow, then we need to add also a 1     
        overflow= ((*result).value[(*result).n_elements-1-i]<=temp)?  1:0;    //check for overflow
    }

    //if *this had more elements than n but there is a pendand overflow, need to add it
    for(i; overflow && i<n_elements; ++i){
        unsigned short temp= value[n_elements-1-i];  //need to save for future check of overflow
        (*result).value[(*result).n_elements-1-i]= value[n_elements-1-i];
        (*result).value[(*result).n_elements-1-i]+= 1;   //if there was an overflow, then we need to add also a 1
        overflow= ((*result).value[(*result).n_elements-1-i]<=temp)?  1:0;    //check for overflow
    }

    for(i; overflow && i<(*result).n_elements; ++i){  //need to sum only untill we have overflow but remain in the boundaries
        unsigned short temp= (*result).value[(*result).n_elements-1-i];  //need to save for future check of overflow
        (*result).value[(*result).n_elements-1-i]+= 1;   //if there was an overflow, then we need to add also a 1
        overflow= ((*result).value[(*result).n_elements-1-i]<=temp)?  1:0;    //check for overflow
    }

    if(overflow){   //if reached the end but with a pendant overflow, then need to increase the number of elements

        vector <unsigned short> new_value= (*result).get_new_value((*result).n_elements+1);  //obtain a new value field populated as the previous one
        new_value[0]= 1;

        (*result).n_elements++;
        ((*result).value).resize(n_elements);
        for(int i=0; i<n_elements; i++)   (*result).value[i]= new_value[i];    

        new_value.~vector();
    }

    return result;
}

/**
 * @brief Overwrite of the += operator with an unsigned short.
 */

void myInt::operator += (const unsigned short add){

    myInt* temp= new myInt((unsigned short) add, 1);
    *this += *temp;

    (*temp).~myInt();
}

/**
 * @brief Overwrite of the += operator with an unsigned int.
 */

void myInt::operator += (const unsigned int add){

    myInt* temp= new myInt((unsigned int) add, 1);
    *this += *temp;

    (*temp).~myInt();
}

/**
 * @brief Overwrite of the += operator with an unsigned long long.
 */

void myInt::operator += (const unsigned long long add){

    myInt* temp= new myInt((unsigned long long) add, 1);
    *this += *temp;

    (*temp).~myInt();
}

/**
 * @brief Overwrite of the -= operator with a myInt object. n must be lower than *this. 
 */

void myInt::operator -= (const myInt n){

    if(*this < n)   puts("ERROR!! SUBTRACTING A LARGER NUMBER!");
    else if(*this == n){
        for(short i=0; i<n_elements; ++i)   value[i]= 0;
    }else{
        short underflow= 0;
        short i=0;

        for(i; i<n.n_elements; ++i){    //stop at n.n_elements because we already know that it is lower or equal than *this.n_elements
            unsigned short temp= value[n_elements-1-i];
            value[n_elements-1-i]-= n.value[n.n_elements-1-i];
            value[n_elements-1-i]-= 1*underflow;
            underflow= (value[n_elements-1-i]>=temp)?  1:0;    //check for underflow
        }

        if(underflow){   //if we had a pendant underflow we need to subtract
            for(i; underflow; ++i){ //do not check for limit as we already know that *this was greater than n, so at some point we will have no underflow
                unsigned short temp= value[n_elements-1-i];
                value[n_elements-1-i]-= 1;
                underflow= (value[n_elements-1-i]>=temp)?  1:0;    //check for underflow
            }
        }
    }    
}

/**
 * @brief Overwrite of the -= operator with a unsigned short. n must be lower than *this. 
 */

void myInt::operator -= (const unsigned short sub){

    if(n_elements == 1 && value[0] < sub)   puts("ERROR!! SUBTRACTING A LARGER NUMBER!");
    else{
        short underflow= 0;
        unsigned short temp= value[n_elements-1];
        value[n_elements-1]-= sub;
        underflow= (value[n_elements-1]>=temp)?  1:0;    //check for underflow

        if(underflow){   //if we had a pendant underflow we need to subtract
            for(short i= 1; underflow; ++i){ //do not check for limit as we already know that *this was greater than n, so at some point we will have no underflow
                unsigned short temp= value[n_elements-1-i];
                value[n_elements-1-i]-= 1;
                underflow= (value[n_elements-1-i]>=temp)?  1:0;    //check for underflow
            }
        }
    }
}

/**
 * @brief Overwrite of the - operator. n must be lower than *this. 
 */

myInt* myInt::operator - (const myInt n){

    unsigned short new_n_elements= 0;
    
    if(*this < n){
        puts("ERROR!! SUBTRACTING A LARGER NUMBER!");
        myInt* result= new myInt();
    }
    else if(*this == n){
        myInt* result= new myInt((unsigned short) (n_elements*16), 0);
        return result;
    }else{

        new_n_elements= (n_elements < n.n_elements)?  n.n_elements : n_elements;
        myInt* result= new myInt((unsigned short) (new_n_elements*16), 0);

        for(unsigned short i=n.n_elements; i<n_elements; ++i){
            (*result).value[(*result).n_elements-1-i]= value[n_elements-1-i];
        }
        
        unsigned short underflow= 0;
        unsigned short i=0;

        for(i; i<n.n_elements; ++i){    //stop at n.n_elements because we already know that it is lower or equal than *this.n_elements
            
            unsigned short temp= value[n_elements-1-i];
            (*result).value[(*result).n_elements-1-i]= value[n_elements-1-i]-n.value[n.n_elements-1-i];
            (*result).value[(*result).n_elements-1-i]-= 1*underflow;
            underflow= ((*result).value[(*result).n_elements-1-i]>=temp)?  1:0;    //check for underflow
        }

        if(underflow){   //if we had a pendant underflow we need to subtract
            for(i; underflow; ++i){ //do not check for limit as we already know that *this was greater than n, so at some point we will have no underflow
                
                unsigned short temp= value[n_elements-1-i];
                (*result).value[(*result).n_elements-1-i]-= 1;
                underflow= ((*result).value[(*result).n_elements-1-i]>=temp)?  1:0;    //check for underflow
            }
        }

        return result;
    }
}

/**
 * @brief Overwrite of the *= operator with a myInt object. 
 */

void myInt::operator *= (const myInt n){
    
    //Initialization for storing the partial results
    short n_columns= n_elements+n.n_elements;
    unsigned short matrix_result[n.n_elements][n_columns];
    for(short i=0; i<n.n_elements; ++i){
        for(short j=0; j<n_columns; ++j){
            matrix_result[i][j]= 0;
        }
    }

    for(short i=0; i<n.n_elements; i++){  //scan elements of n. i also represents the first row of the matrix
        unsigned short a= n.value[n.n_elements-1-i]; //save the number for additions

        for(short j=0; j<n_elements; ++j){    //scan elements of *this. j also represents the column of the matrix
            short overflow= 0;

            for(unsigned short k=0; k<value[n_elements-1-j]; ++k){   //sum n.value[n.elements-1-i] for value[n_elements-1-j] times
                unsigned short temp= matrix_result[i][n_columns-1-j-i];
                matrix_result[i][n_columns-1-j-i]+= a; //sum the considered number
                if(temp>matrix_result[i][n_columns-1-j-i]) overflow++;       
            }

            matrix_result[i][n_columns-2-j-i]+= overflow;  //store the overflow that need to sum to the following cell
        }
    }

    //now the matrix of partial results is populated, need to perform the sum in columns. It is likely that the size of value must be increased, so we need a new container of the correct size.
    
    //determination of the correct size
    short new_size= 0;
    for(short i=0; i<n_columns; ++i){
        if(matrix_result[n.n_elements-1][i] != 0){
            new_size= n_columns-i;
            break;
        }
    }
    if(new_size==0)  new_size++; //if new_size is still 0, this means we multiplied by 0, so we need to increase in order to have a meaningful size

    n_elements= new_size;
    value.resize(n_elements);
    for(int i=0; i<n_elements; i++)   value[i]= 0;

    short overflow= 0;
    for(short i=0; i<n_columns; ++i){   //scan the columns
        overflow= 0;
        for(short j=0; j<n.n_elements; j++){    //scan the rows

            unsigned short temp= value[n_elements-1-i];
            value[n_elements-1-i]+= matrix_result[j][n_columns-1-i];
            if(temp>value[n_elements-1-i]) overflow++;            
        }
        
        if(i<(n_columns-1)) //need to check we are in boundaries, otherwise we go out of the boundaries
            value[n_elements-2-i]+= overflow;
    }

    if(overflow){   //there is a pendant overflow
        vector <unsigned short> new_value= get_new_value(n_elements+1);  //obtain a new value field populated as the previous one
        new_value[0]= overflow;

        n_elements++;
        value.resize(n_elements);
        for(int i=0; i<n_elements; i++)   value[i]= new_value[i];    

        new_value.~vector();
    }
}

/**
 * @brief Overwrite of the *= operator with an unsigned short number. 
 */

void myInt::operator *= (const unsigned short mult){

    vector <unsigned short> new_value(n_elements, 0);  //obtain a new value container 0 populated
    short overflow= 0;

    for(short j=0; j<n_elements; ++j){    //scan elements of *this
        overflow= 0;

        for(unsigned short k=0; k<value[n_elements-1-j]; ++k){   //sum mult for value[n_elements-1-j] times
            
            unsigned short temp= new_value[n_elements-1-j];
            new_value[n_elements-1-j]+= mult; //sum the considered number
            if(temp>new_value[n_elements-1-j]) overflow++;       
        }

        if(j<n_elements-1)  new_value[n_elements-2-j]+= overflow;  //store the overflow that need to sum to the following cell
    }

    for(int i=0; i<n_elements; i++)   value[i]= new_value[i];
    new_value.~vector();

    if(overflow){   //there is a pendant overflow
        new_value= get_new_value(n_elements+1);  //obtain a new value field populated as the previous one
        new_value[0]= overflow;

        n_elements++;
        value.resize(n_elements);
        for(int i=0; i<n_elements; i++)   value[i]= new_value[i];    

        new_value.~vector();
    }
}

/**
 * @brief Overwrite of the * operator. 
 */

myInt* myInt::operator * (const myInt n){

    //Initialization for storing the partial results
    short n_columns= n_elements+n.n_elements;
    unsigned short matrix_result[n.n_elements][n_columns];
    for(short i=0; i<n.n_elements; ++i){
        for(short j=0; j<n_columns; ++j){
            matrix_result[i][j]= 0;
        }
    }

    for(short i=0; i<n.n_elements; i++){  //scan elements of n. i also represents the first row of the matrix
        unsigned short a= n.value[n.n_elements-1-i]; //save the number for additions

        for(short j=0; j<n_elements; ++j){    //scan elements of *this. j also represents the column of the matrix
            short overflow= 0;

            for(unsigned short k=0; k<value[n_elements-1-j]; ++k){   //sum n.value[n.elements-1-i] for value[n_elements-1-j] times
                unsigned short temp= matrix_result[i][n_columns-1-j-i];
                matrix_result[i][n_columns-1-j-i]+= a; //sum the considered number
                if(temp>matrix_result[i][n_columns-1-j-i]) overflow++;       
            }

            matrix_result[i][n_columns-2-j-i]+= overflow;  //store the overflow that need to sum to the following cell
        }
    }

    //determination of the correct size
    short new_size= 0;
    for(short i=0; i<n_columns; ++i){
        if(matrix_result[n.n_elements-1][i] != 0){
            new_size= n_columns-i;
            break;
        }
    }
    if(new_size==0)  new_size++; //if new_size is still 0, this means we multiplied by 0, so we need to increase in order to have a meaningful size

    myInt* result= new myInt((unsigned short) (new_size*16), 0);

    short overflow= 0;
    for(short i=0; i<n_columns; ++i){   //scan the columns
        overflow= 0;
        for(short j=0; j<n.n_elements; j++){    //scan the rows

            unsigned short temp= (*result).value[(*result).n_elements-1-i];
            (*result).value[(*result).n_elements-1-i]+= matrix_result[j][n_columns-1-i];
            if(temp>(*result).value[(*result).n_elements-1-i]) overflow++;            
        }
        
        if(i<(n_columns-1)) //need to check we are in boundaries, otherwise we go out of the boundaries
            (*result).value[(*result).n_elements-2-i]+= overflow;
    }

    if(overflow){   //there is a pendant overflow
        vector <unsigned short> new_value= (*result).get_new_value(n_elements+1);  //obtain a new value field populated as the previous one
        new_value[0]= overflow;

        (*result).n_elements++;
        ((*result).value).resize(n_elements);
        for(int i=0; i<n_elements; i++)   (*result).value[i]= new_value[i];    

        new_value.~vector();
    }

    return result;
}

//------------------------- COMPARISON OPERATORS -------------------------------

/**
 * @brief Overwrite of the == operator.
 * 
 * @return 1 if true, 0 otherwise.
 */

bool myInt::operator == (const myInt n){

    unsigned short true_this=0, true_n=0;   //position of the first non-zero elements. As it is possible that there are few 0 elements in front derived from operations.
    
    for(true_this; value[true_this]==0 && true_this < n_elements; ++true_this);
    for(true_n; n.value[true_n]==0 && true_n < n.n_elements; ++true_n);

    if((n_elements - true_this) != (n.n_elements - true_n))	return false;
	else{
		//number of true elements is the same. Then check element by element
        for(unsigned short i=0; i<n_elements-true_this; ++i){
            if(value[true_this+i] != n.value[true_n+i]) return false;
        }

        //if exit from the loop then all the elements were the same
        return true;
    }	
}

/**
 * @brief Overwrite of the != operator.
 * 
 * @return 1 if true, 0 otherwise.
 */

bool myInt::operator != (const myInt n){
    return !(*this == n);
}

/**
 * @brief Overwrite of the > operator.
 * 
 * @return 1 if true, 0 otherwise.
 */
bool myInt::operator > (const myInt n){

    unsigned short true_this=0, true_n=0;   //position of the first non-zero elements. As it is possible that there are few 0 elements in front derived from operations.
    
    for(true_this; value[true_this]==0 && true_this < n_elements; ++true_this);
    for(true_n; n.value[true_n]==0 && true_n < n.n_elements; ++true_n);

    //at this point true_this and true_n represent the position of the first non-zero element

    //First compare n_elements or the number of non-0 elements. If equal, then check element by element
	if((n_elements - true_this) > (n.n_elements - true_n))  return true;
	if((n_elements - true_this) < (n.n_elements - true_n))	return false;
	else{
		//number of true elements is the same. Then check element by element
        for(unsigned short i=0; i<n_elements-true_this; ++i){
            if(value[true_this+i] > n.value[true_n+i]) return true;
            else if(value[true_this+i] < n.value[true_n+i])   return false;   //if same number of element but the last is not greater, then it is lower or equal.

            //if not then they are euqual and so need to go on
        }
        return false;
    }
}

/**
 * @brief Overwrite of the >= operator.
 * 
 * @return 1 if true, 0 otherwise.
 */

bool myInt::operator >= (const myInt n){

    unsigned short true_this=0, true_n=0;   //position of the first non-zero elements. As it is possible that there are few 0 elements in front derived from operations.
    
    for(true_this; value[true_this]==0 && true_this < n_elements; ++true_this);
    for(true_n; n.value[true_n]==0 && true_n < n.n_elements; ++true_n);

    //at this point true_this and true_n represent the position of the first non-zero element

    //First compare n_elements or the number of non-0 elements. If equal, then check element by element
	if((n_elements - true_this) > (n.n_elements - true_n))  return true;
	if((n_elements - true_this) < (n.n_elements - true_n))	return false;
	else{
		//number of true elements is the same. Then check element by element
        for(unsigned short i=0; i<n_elements-true_this; ++i){
            if(value[true_this+i] > n.value[true_n+i]) return true;
            else if(value[true_this+i] < n.value[true_n+i])   return false;   //if same number of element but the last is not greater, then it is lower or equal.

            //if not then they are euqual and so need to go on
        }
        return true;
    }
}

/**
 * @brief Overwrite of the < operator.
 * 
 * @return 1 if true, 0 otherwise.
 */

bool myInt::operator < (const myInt n){

    unsigned short true_this=0, true_n=0;   //position of the first non-zero elements. As it is possible that there are few 0 elements in front derived from operations.
    
    for(true_this; value[true_this]==0 && true_this < n_elements; ++true_this);
    for(true_n; n.value[true_n]==0 && true_n < n.n_elements; ++true_n);


	//First compare n_elements. If equal, then check element by element
	if((n_elements - true_this) < (n.n_elements - true_n))	return true;
	if((n_elements - true_this) > (n.n_elements - true_n))	return false;
	else{
		//number of true elements is the same. Then check element by element
        for(unsigned short i=0; i<n_elements-true_this; ++i){
            if(value[true_this+i] < n.value[true_n+i]) return true;
            else if(value[true_this+i] > n.value[true_n+i])   return false;   //if same number of element but the last is not greater, then it is lower or equal.

            //if not then they are euqual and so need to go on
        }
        return false;
    }
}

/**
 * @brief Overwrite of the <= operator.
 * 
 * @return 1 if true, 0 otherwise.
 * 
 */

bool myInt::operator <= (const myInt n){

	unsigned short true_this=0, true_n=0;   //position of the first non-zero elements. As it is possible that there are few 0 elements in front derived from operations.
    
    for(true_this; value[true_this]==0 && true_this < n_elements; ++true_this);
    for(true_n; n.value[true_n]==0 && true_n < n.n_elements; ++true_n);


	//First compare n_elements. If equal, then check element by element
	if((n_elements - true_this) < (n.n_elements - true_n))	return true;
	if((n_elements - true_this) > (n.n_elements - true_n))	return false;
	else{
		//number of true elements is the same. Then check element by element
        for(unsigned short i=0; i<n_elements-true_this; ++i){
            if(value[true_this+i] < n.value[true_n+i]) return true;
            else if(value[true_this+i] > n.value[true_n+i])   return false;   //if same number of element but the last is not greater, then it is lower or equal.

            //if not then they are euqual and so need to go on
        }
        return true;
    }
}

/**
 * @brief Overwrite of the << operator with an unsigned short number. 
 */

myInt* myInt::operator << (const unsigned short shift){

    unsigned short n_bits= 16;    //number of bits per element    
    unsigned short new_size= (shift%n_bits > 0)?    n_elements+(shift/n_bits)+1 : n_elements+(shift/n_bits);    //first thing is to resize the container    
    
    myInt* result= new myInt((unsigned short) (new_size*16), 0);  //obtain a new value container resized

    //do not update n_elements yet as we need to use the old value

    if(shift%n_bits == 0){  //if the shift is a multiple of the elements size, then we just need to copy element by element
        for(unsigned short i=0; i<n_elements; ++i){
            (*result).value[i] = value[i];
        }
    }else{    
        for(unsigned short i=0; i<n_elements; ++i){

            unsigned short temp= value[i];
            (*result).value[i]+= temp >> (n_bits-(shift%n_bits));
            (*result).value[i+1]+= temp << (shift%n_bits);
        }
    }

    return result;
}

/**
 * @brief Implements the round left shift
 * 
 * @param shift: number of bit shift 
 */

myInt* myInt::round_left_shift(const unsigned short shift){

    unsigned short n_bits= 16;    //number of bits per element    
    unsigned short new_size= (shift%n_bits > 0)?    n_elements+(shift/n_bits)+1 : n_elements+(shift/n_bits);    //first thing is to resize the container    
    
    vector <unsigned short> new_value(new_size, 0);  //obtain a new value container resized
    myInt* result= new myInt((unsigned short) (n_elements*16), 0);  //obtain a new value container resized

    if(shift%n_bits == 0){  //if the shift is a multiple of the elements size, then we just need to copy element by element
        for(unsigned short i=0; i<n_elements; ++i){
            new_value[i] = value[i];
        }
    }else{    
        for(unsigned short i=0; i<n_elements; ++i){

            unsigned short temp= value[i];
            new_value[i]+= temp >> (n_bits-(shift%n_bits));
            new_value[i+1]+= temp << (shift%n_bits);
        }
    }

    //up to now is like a normal shift. So we need to move back the first elements 

    for(unsigned short i=0; i<new_size-n_elements; ++i){    //loop for moving back the elements 
        new_value[n_elements+i]+= new_value[i];
    }

    for(unsigned short i=new_size-n_elements; i<new_size; ++i){ //loop for copying the elements correctly 
        (*result).value[i-(new_size-n_elements)]= new_value[i];
    }

    new_value.~vector();

    return result;
}

/**
 * @brief Overwrite of the >> operator. 
 */

myInt* myInt::operator >> (const unsigned short shift){
    
    unsigned short n_bits= 16;    //number of bits per element
    unsigned short new_size= (shift/n_bits <n_elements)?    n_elements-(shift/n_bits) : 0;
    
    myInt* result= new myInt((unsigned short) (new_size*16), 0);

    if(new_size!=0){
        if(shift%n_bits == 0){
            for(unsigned short i=0; i<new_size; ++i){
                (*result).value[i]= value[i];
            }
        }else{
            for(unsigned short i=0; i<new_size; ++i){
                unsigned short temp= value[i];

                if(i<new_size-1)    (*result).value[i+1]+= temp << n_bits - (shift%n_bits);
                (*result).value[i]+= temp >> shift%n_bits;
            }
        }

    }        
    
    return result;
}

/**
 * @brief Implements the round right shift
 * @note Need to keep in mind that the first element is fulfilled with 0s to complete the size of the short. These 0s will be considered in the shift.
 */

myInt* myInt::round_right_shift(const unsigned short shift){

    //the working idea is the same of the left shift -> create a larger container, perform a normal shift, move back the bits than need to be round, and then consider just the element of interest
    //the difference is that now the shift is to the right
        
    unsigned short n_bits= 16;    //number of bits per element    
    unsigned short new_size= (shift%n_bits > 0)?    n_elements+(shift/n_bits)+1 : n_elements+(shift/n_bits);    //first thing is to resize the container    
    
    vector <unsigned short> new_value(new_size, 0);  //obtain a new value container resized

    myInt* result= new myInt((unsigned short)(n_elements*16), 0);

    if(shift%n_bits == 0){  //if the shift is a multiple of the elements size, then we just need to copy element by element
        for(unsigned short i=0; i<n_elements; ++i){
            new_value[new_size-1-i] = value[n_elements-1-i];
        }
    }else{    
        for(unsigned short i=0; i<n_elements; ++i){

            unsigned short temp= value[n_elements-1-i];
            new_value[new_size-1-i]+= temp << (n_bits-(shift%n_bits));
            new_value[new_size-1-i-1]+= temp >> (shift%n_bits);
        }
    }

    //up to now is like a normal shift. So we need to move back the first elements 

    for(unsigned short i=0; i<new_size-n_elements; ++i){    //loop for moving back the elements 
        new_value[i]+= new_value[new_size-1-i];
    }

    for(unsigned short i=0; i<n_elements; ++i){ //loop for copying the elements correctly 
        (*result).value[i]= new_value[i];
    }

    new_value.~vector();

    return result;
}


//------------------------- SHIFT OPERATIONS -----------------------------------

/**
 * @brief Overwrite of the <<= operator with an unsigned short number. 
 */

void myInt::operator <<= (const unsigned short shift){
    
    unsigned short n_bits= 16;    //number of bits per element    
    unsigned short new_size= (shift%n_bits > 0)?    n_elements+(shift/n_bits)+1 : n_elements+(shift/n_bits);    //first thing is to resize the container    
    
    vector <unsigned short> new_value(new_size, 0);  //obtain a new value container resized

    //do not update n_elements yet as we need to use the old value

    if(shift%n_bits == 0){  //if the shift is a multiple of the elements size, then we just need to copy element by element
        for(unsigned short i=0; i<n_elements; ++i){
            new_value[i] = value[i];
        }
    }else{    
        for(unsigned short i=0; i<n_elements; ++i){

            unsigned short temp= value[i];
            new_value[i]+= temp >> (n_bits-(shift%n_bits));
            new_value[i+1]+= temp << (shift%n_bits);
        }
    }

    n_elements= new_size;
    value.resize(n_elements);
    for(int i=0; i<n_elements; i++)   value[i]= new_value[i];    

    new_value.~vector();
}

/**
 * @brief Implements the round left shift on the element itself
 * 
 * @param shift: number of bit shift 
 */

void myInt::self_round_left_shift(const unsigned short shift){

    unsigned short n_bits= 16;    //number of bits per element    
    unsigned short new_size= (shift%n_bits > 0)?    n_elements+(shift/n_bits)+1 : n_elements+(shift/n_bits);    //first thing is to resize the container    
    
    vector <unsigned short> new_value(new_size, 0);  //obtain a new value container resized

    if(shift%n_bits == 0){  //if the shift is a multiple of the elements size, then we just need to copy element by element
        for(unsigned short i=0; i<n_elements; ++i){
            new_value[i] = value[i];
        }
    }else{    
        for(unsigned short i=0; i<n_elements; ++i){

            unsigned short temp= value[i];
            new_value[i]+= temp >> (n_bits-(shift%n_bits));
            new_value[i+1]+= temp << (shift%n_bits);
        }
    }

    //up to now is like a normal shift. So we need to move back the first elements 

    for(unsigned short i=0; i<new_size-n_elements; ++i){    //loop for moving back the elements 
        new_value[n_elements+i]+= new_value[i];
    }

    for(unsigned short i=new_size-n_elements; i<new_size; ++i){ //loop for copying the elements correctly 
        value[i-(new_size-n_elements)]= new_value[i];
    }

    new_value.~vector();
}

/**
 * @brief Overwrite of the >>= operator with an unsigned short number. 
 */

void myInt::operator >>= (const unsigned short shift){

    unsigned short n_bits= 16;    //number of bits per element
    unsigned short new_size= (shift/n_bits <n_elements)?    n_elements-(shift/n_bits) : 0;
    
    vector <unsigned short> new_value(new_size, 0);  //obtain a new value container resized
    
    if(new_size!=0){
        if(shift%n_bits == 0){
            for(unsigned short i=0; i<new_size; ++i){
                new_value[i]= value[i];
            }
        }else{
            for(unsigned short i=0; i<new_size; ++i){
                unsigned short temp= value[i];

                if(i<new_size-1)    new_value[i+1]+= temp << n_bits - (shift%n_bits);
                new_value[i]+= temp >> shift%n_bits;
            }
        }

    }        
    
    n_elements= new_size;
    value.resize(n_elements);
    for(int i=0; i<n_elements; i++)   value[i]= new_value[i];    

    new_value.~vector();
}

/**
 * @brief Implements the round right shift on the element itself
 * @note Need to keep in mind that the first element is fulfilled with 0s to complete the size of the short. These 0s will be considered in the shift.
 */

void myInt::self_round_right_shift(const unsigned short shift){

    //the working idea is the same of the left shift -> create a larger container, perform a normal shift, move back the bits than need to be round, and then consider just the element of interest
    //the difference is that now the shift is to the right
        
    unsigned short n_bits= 16;    //number of bits per element    
    unsigned short new_size= (shift%n_bits > 0)?    n_elements+(shift/n_bits)+1 : n_elements+(shift/n_bits);    //first thing is to resize the container    
    
    vector <unsigned short> new_value(new_size, 0);  //obtain a new value container resized

    if(shift%n_bits == 0){  //if the shift is a multiple of the elements size, then we just need to copy element by element
        for(unsigned short i=0; i<n_elements; ++i){
            new_value[new_size-1-i] = value[n_elements-1-i];
        }
    }else{    
        for(unsigned short i=0; i<n_elements; ++i){

            unsigned short temp= value[n_elements-1-i];
            new_value[new_size-1-i]+= temp << (n_bits-(shift%n_bits));
            new_value[new_size-1-i-1]+= temp >> (shift%n_bits);
        }
    }

    //up to now is like a normal shift. So we need to move back the first elements 

    for(unsigned short i=0; i<new_size-n_elements; ++i){    //loop for moving back the elements 
        new_value[i]+= new_value[new_size-1-i];
    }

    for(unsigned short i=0; i<n_elements; ++i){ //loop for copying the elements correctly 
        value[i]= new_value[i];
    }

    new_value.~vector();
}



//------------------------- BINARY OPERATORS ----------------------------------

/**
 * @brief Overwrite the &= operator. If two elmeents are no longer the same, the operation is considered as adding 0s in front.
 */

void myInt::operator &= (const myInt n){

    if(n_elements < n.n_elements){  //if we are AND with a value with more elements

        vector <unsigned short> new_value(n.n_elements, 0);
        
        for(unsigned short i=0; i<n_elements; ++i)
            new_value[n.n_elements-1-i]= value[n_elements-1-i] & n.value[n.n_elements-1-i];
        
        //'0' AND 'something' = '0' ==> no need to copy the rest since we recall new_value is 0 initiated

        n_elements= n.n_elements;
        value.resize(n_elements);
        for(int i=0; i<n_elements; i++)   value[i]= new_value[i];    

        new_value.~vector();

    }else{

        unsigned short i=0;
        for(i; i<n.n_elements; ++i){
            value[n_elements-1-i] &= n.value[n.n_elements-1-i];
        }

        for(i; i<n_elements; ++i)   value[n_elements-1-i]= 0;   //need to set to 0 everything that remains apart
    }
} 

/**
 * @brief Overwrite the & operator. If two elmeents are no longer the same, the operation is considered as adding 0s in front.
 */

myInt* myInt::operator & (const myInt n){

    unsigned short min_n_elements= (n_elements < n.n_elements)? n_elements : n.n_elements;
    unsigned short max_n_elements= (n_elements > n.n_elements)? n_elements : n.n_elements;

    myInt* result= new myInt((unsigned short) (max_n_elements*16), 0);

    for(unsigned short i=0; i<min_n_elements; ++i)
        (*result).value[(*result).n_elements-1-i]= value[n_elements-1-i] & n.value[n.n_elements-1-i];

    return result;
}

/**
 * @brief Overwrite the |= operator. If two elmeents are no longer the same, the operation is considered as adding 0s in front.
 */

void myInt::operator |= (const myInt n){

    if(n_elements < n.n_elements){  //if we are OR with a value with more elements

        vector <unsigned short> new_value(n.n_elements, 0);
        
        for(unsigned short i=0; i<n_elements; ++i)  
            new_value[n.n_elements-1-i]= value[n_elements-1-i] | n.value[n.n_elements-1-i];
        
        for(unsigned short i=n_elements; i<n.n_elements; ++i)   //'0' OR 'something' = 'something' ==> need to copy what remains out from n 
            new_value[n.n_elements-1-i]= n.value[n.n_elements-1-i];

        n_elements= n.n_elements;
        value.resize(n_elements);
        for(int i=0; i<n_elements; i++)   value[i]= new_value[i];    

        new_value.~vector();

    }else{

        unsigned short i=0;
        for(i; i<n.n_elements; ++i)
            value[n_elements-1-i] |= n.value[n.n_elements-1-i];        
    }
}

/**
 * @brief Overwrite the | operator. If two elmeents are no longer the same, the operation is considered as adding 0s in front.
 */

myInt* myInt::operator | (const myInt n){

    unsigned short min_n_elements= (n_elements < n.n_elements)? n_elements : n.n_elements;
    unsigned short max_n_elements= (n_elements > n.n_elements)? n_elements : n.n_elements;

    myInt* result= new myInt((unsigned short) (max_n_elements*16), 0);

    for(unsigned short i=0; i<min_n_elements; ++i)
        (*result).value[(*result).n_elements-1-i]= value[n_elements-1-i] | n.value[n.n_elements-1-i];
    
    if(n_elements < n.n_elements){
        for(unsigned short i=min_n_elements; i<n.n_elements; ++i)
            (*result).value[(*result).n_elements-1-i]= n.value[n.n_elements-1-i];

    }else{
        for(unsigned short i=min_n_elements; i<n_elements; ++i)
            (*result).value[(*result).n_elements-1-i]= value[n_elements-1-i];
    }

    return result;
}

/**
 * @brief Overwrite the ^= operator. If two elmeents are no longer the same, the operation is considered as adding 0s in front.
 */

void myInt::operator ^= (const myInt n){

    if(n_elements < n.n_elements){  //if we are AND with a value with more elements

        vector <unsigned short> new_value(n.n_elements, 0);
        
        for(unsigned short i=0; i<n_elements; ++i)
            new_value[n.n_elements-1-i]= value[n_elements-1-i] ^ n.value[n.n_elements-1-i];
        
        //'0' XOR 'something' = 'something' ==> no need to copy the rest since we recall new_value is 0 initiated
        for(unsigned short i=n_elements; i<n.n_elements; ++i)
            new_value[n.n_elements-1-i]= n.value[n.n_elements-1-i];

        n_elements= n.n_elements;
        value.resize(n_elements);
        for(int i=0; i<n_elements; i++)   value[i]= new_value[i];    

        new_value.~vector();

    }else{

        unsigned short i=0;
        for(i; i<n.n_elements; ++i){
            value[n_elements-1-i] ^= n.value[n.n_elements-1-i];
        }
    }
}

/**
 * @brief Overwrite the ^ operator. If two elmeents are no longer the same, the operation is considered as adding 0s in front.
 */

myInt* myInt::operator ^ (const myInt n){

    unsigned short min_n_elements= (n_elements < n.n_elements)? n_elements : n.n_elements;
    unsigned short max_n_elements= (n_elements > n.n_elements)? n_elements : n.n_elements;

    myInt* result= new myInt((unsigned short) (max_n_elements*16), 0);

    for(unsigned short i=0; i<min_n_elements; ++i)
        (*result).value[(*result).n_elements-1-i]= value[n_elements-1-i] ^ n.value[n.n_elements-1-i];

    if(n_elements < n.n_elements){
        for(unsigned short i=min_n_elements; i<n.n_elements; ++i)
            (*result).value[(*result).n_elements-1-i]= n.value[n.n_elements-1-i];

    }else{
        for(unsigned short i=min_n_elements; i<n_elements; ++i)
            (*result).value[(*result).n_elements-1-i]= value[n_elements-1-i];
    }

    return result;
}

/**
 * @brief Overwrite the ~ (bitwise) operator. 
 */

myInt* myInt::operator ~(){

    myInt* result= new myInt((unsigned short) (n_elements*16), 0);

    for(unsigned short i=0; i<n_elements; ++i){
        (*result).value[n_elements-1-i] = ~value[n_elements-1-i];
    }

    return result;
}


//------------------------- UTILITY FUNCTIONS ----------------------------------

/**
 * @brief Creates a new value field with a new size, copying the current one into the new one. The additional elements are fulfilled with 0
 * 
 * @param new_size: number of elements for the new container
 * 
 * @return new_value: new array of unsigned short with @param new_size elements 
 */

vector <unsigned short> myInt::get_new_value(short new_size){

    vector <unsigned short> new_value(new_size, 0);

    short i=0;
    for(i; i<n_elements; ++i){
        new_value[new_size-1-i]= value[n_elements-1-i]; //we copy from the last element as we recall the element 0 is the first one, so if we want to expand, new elements are located at the beginning
    }

    for(i; i<new_size; ++i){
        new_value[new_size-i-1]= 0; //fulfill the remaining with zeros
    }

    return new_value;
}

/**
 * @brief Check the endianess of the system. This is needed for a correct initialization.
 */

bool myInt::check_endian(){

    short prova= 1;

    char *point= (char*) &prova;
    if(point)   return 1;
    else    return 0;
}

#endif /*MYINT_H*/