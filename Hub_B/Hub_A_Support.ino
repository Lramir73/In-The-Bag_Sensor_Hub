/*  Most of the Hub_A Softserial support code is in
 *  the libaray "SerialB2A" 
 */

/***** CASE 0 *****/
void Reset()
{
  //restore the default settings
  //pass the default config to A.
  //put the flag pin low.
}

/***** CASE 24 *****/
void TurnOffFlag()
{
  digitalWrite(FLAG, LOW);
  Clock.checkIfAlarm(1);
}
