#include "Debug.h"
#include "iFloat.h"

/** @file iFloat.c
 *  @brief You will modify this file and implement nine functions
 *  @details Your implementation of the functions defined in iFloat.h.
 *  You may add other function if you find it helpful. Added function
 *  should be declared <b>static</b> to indicate they are only used
 *  within this file.
 *  <p>
 *  @author THADISTHE BEST AT 8 BALL POOL
 *
 */

/* declaration for useful function contained in testFloat.c */
const char* getBinary (iFloat_t value);

iFloat_t floatGetSign (iFloat_t x) {
  if(x >= 0){
	return 0;
}
  return 1;
}

iFloat_t floatGetExp (iFloat_t x) {
  int expMask = (1 << (BITS_EXP + BITS_MANT)) - 1;
  int valMask = (1 << (BITS_MANT)) - 1;
  int exp;

  expMask = expMask - valMask;
   // printf("%d \n",expMask);
   // MASK WORKS CORRECTLY HERE 
  exp =  expMask & x;
  exp = (exp >> BITS_MANT);

  return exp;
}

iFloat_t floatGetVal (iFloat_t x) {
  int valMask = (1 << (BITS_MANT)) - 1;
  int value = x & valMask;

     // adding the implicit one
  value += (1<< BITS_MANT); 
    
    // checking if we need 2's comp on this bad boii
  if(floatGetSign(x) == 0){
	return value;
}
  else{
	value = ~value;
	value += 1;
	return value;

}
}

/** @todo Implement based on documentation contained in iFloat.h */
void floatGetAll(iFloat_t x, iFloat_t* sign, iFloat_t*exp, iFloat_t* val) { 
   *sign = floatGetSign(x);
   *exp = floatGetExp(x);
   *val = floatGetVal(x);


}
// TODO COME BACK HEREEEEEEEEEEEEE|
//
//
//
/** @todo Implement based on documentation contained in iFloat.h */
iFloat_t floatLeftMost1 (iFloat_t bits) {
  int mask = (1 <<( BITS -1));
  int checker = 1;
  int counter = BITS - 1;

//  printf("mask for left most %d \n", mask);
//  printf("counter check %d \n", counter);

 
  if(bits == 0){
	return -1;
}
  else{
     for(int i  = (BITS - 1); i >=0; i --){
       checker = mask & bits;
         if(checker != 0){
	    return counter;
     }			
       mask = mask >>1;
       counter = counter - 1;
    }
  }
 return counter;
}

/** @todo Implement based on documentation contained in iFloat.h */
iFloat_t floatAbs (iFloat_t x) {
  int maskAbs = (1 << (BITS - 1)) - 1;
  int value = x & maskAbs;
  
 // printf("checking abs value mask %d \n",maskAbs);

  if(x == 0){
	return 0;
}
  else{
	return value;
}

}

/** @todo Implement based on documentation contained in iFloat.h */
iFloat_t floatNegate (iFloat_t x) {
  // TODO LEFT OFF HERE THIS IS CAKE THO 
  int maskNeg = (1 << (BITS - 1));
  int value = x ^ maskNeg;
  
  debug("checking neg mask %d \n", maskNeg);
   
  if(x == 0){
    return 0;
}
  return value;
}

/** @todo Implement based on documentation contained in iFloat.h */
iFloat_t floatAdd (iFloat_t x, iFloat_t y) { 
  iFloat_t xExponent, xSign ,xValue;
  iFloat_t yExponent, ySign, yValue;
  iFloat_t valR;
  iFloat_t magR = 0;
  iFloat_t expR;
  int expDiff;
  int signR;
// need to change for 32 bit addition
  int binaryPoint = 7;
  int binShiftSize;
  int finalMask = 1;

  
  floatGetAll(x, &xSign, &xExponent, &xValue);
  floatGetAll(y, &ySign, &yExponent, &yValue);

  debug("x exponent %s \n", getBinary(xExponent));
  debug("y exponent %s \n", getBinary(yExponent));
  debug("x value %s \n", getBinary(xValue));
  debug("y value %s \n", getBinary(yValue));
  debug("y sign %s \n", getBinary(ySign));
  debug("x sign %s \n", getBinary(xSign));
 
  if(x == 0){
    return y;
} 
  if(y == 0){
    return x;
}
  if(x == floatNegate(y)){
    return 0;
   }
  if(y == floatNegate(x)){
    return 0;
}

  if(xExponent > yExponent){
    expDiff = xExponent - yExponent;
    expR = xExponent;
    yValue = yValue >> expDiff;
   }
  else{
    expDiff = yExponent - xExponent;
    expR = yExponent;
    xValue = xValue >> expDiff;
   }
  
  debug("Y exponent Difference and balance test %s \n", getBinary(yValue));
  debug("X exponent Difference and balance test %s \n", getBinary(xValue));

  valR = xValue + yValue;
 
  debug("Making sure valR is correct after step 3 %s \n", getBinary(valR));

  if(valR < 0){
    valR = ~valR;
    valR = valR + 1;
    magR = valR;
    signR = 1;
}
  else{
    magR = valR;
    signR = 0;
}
  debug("Mag R check after the 4th step %s \n", getBinary(magR));

//Step 5
  binShiftSize = floatLeftMost1(magR);

  debug("Exp R check before entering step 5 %s \n", getBinary(expR));

  if(binShiftSize > binaryPoint){
    binShiftSize = binShiftSize - binaryPoint;
    magR = magR >> binShiftSize;
    expR = expR + binShiftSize; 

}

  else if(binShiftSize < binaryPoint){
    binShiftSize = binaryPoint - binShiftSize;
    magR = magR << binShiftSize;
    expR = expR - binShiftSize;
}

  debug("Mag R check after the 5th step %s \n", getBinary(magR));
  debug("Exp R check after the 5th step %s \n", getBinary(expR));
  
//STEP 6
  finalMask = (finalMask << (BITS_MANT)) - 1;

  debug("final Mask checker %s \n", getBinary(finalMask));

  signR = signR <<(BITS - 1);
  expR = expR << (BITS_MANT);
  magR = magR & finalMask;
    
  magR = magR | expR;
  magR = magR | signR;

  debug("final result check %s \n", getBinary(magR));

  return magR;
}

/** @todo Implement based on documentation contained in iFloat.h */
iFloat_t floatSub (iFloat_t x, iFloat_t y) {
  return floatAdd(x, floatNegate(y));
}

