
void clearAllBuffers()
{
  memset(Buffer, 0, BUFF_SIZE);   // Clear contents of Buffer
  memset(Command, 0, CMD_SIZE);   // Clear contents of Buffer
  memset(Data, 0, DATA_SIZE);     // Clear contents of Buffer
}

void clearBuffer()
{
  memset(Buffer, 0, BUFF_SIZE);   // Clears main Buffer
}

void clearData()
{
  memset(Data, 0, DATA_SIZE);   // Clears main Buffer
}

void clearSettings()
{
  
}

