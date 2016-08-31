void SeekLine(File* Filename,int LineNum){
  Filename->seek(0);
  Serial.println();
  if(LineNum==0)
    return;
  int i=0;
  while(i<LineNum)
  {
    if(Filename->read()=='\n')
    i++;
  }
}

void MoveCursor(File* Filename){
  while(Filename->read()!='='){
  }
}

void InitailLine(File* Filename,int LineNum){ //In case of any problems when doing the rewrite.
  SeekLine(Filename,LineNum);
  MoveCursor(Filename);
  Filename->print(F("                    "));
  SeekLine(Filename,LineNum);
  MoveCursor(Filename);
}

void ReadUntilSpace(File* Filename){
  int i = 0;
  while(Filename->peek()!=' '){
    SerialB2A.Buffer[i++] = Filename->read();
  }
}
