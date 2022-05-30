void setup()
{
  Serial.begin(9600);
}

void loop()
{
  String message = coord(5,8,2,4);
  Serial.print("le message a envoyé est : ");
  Serial.println(message);
  while(1);
}

String coord(int x1, int y1, int x2, int y2)
{
  String msg = "";

    if (x1 > x2)
    {
      Serial.println("condition 1");
      msg = "" + instruction(msg, "g", 1);
      msg = "" + instruction(msg, "a", abs(x1-x2));
      msg = "" + instruction(msg, "d", 1);
    }

    if (x1 < x2)
    {
      Serial.println("condition 2");
      msg = "" + instruction(msg, "d", 1);
      msg = "" + instruction(msg, "a", abs(x1-x2));
      msg = "" + instruction(msg, "g", 1);
    }

    if (y1 > y2)
    {
      Serial.println("condition 3");
      msg = "" + instruction(msg, "d", 2);
      msg = "" + instruction(msg, "a", abs(y1-y2));
      msg = "" + instruction(msg, "d", 2);
    }

    if (y1 < y2)
    {
      Serial.println("condition 4");
      msg = "" + instruction(msg, "a", abs(y1-y2));
    }

    if (x1 == x2 or y1 == y2)
    {
      Serial.println("condition else");
    }
    
    return msg;
}


String instruction(String msg, String action, int repeats_number)
{
  if (repeats_number >= 100)
  {
    msg = msg + action + repeats_number;
    return msg;
  }
  
  else if (repeats_number >= 10)
  {
   msg = msg + action + "0" + repeats_number;
   return msg;
  }
  
  else if (repeats_number < 10)
  {
    msg = msg + action + "00" + repeats_number;
    return msg;
  }
}
