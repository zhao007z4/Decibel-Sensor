
unsigned int AD_TAB[100];

void setup() 
{
 
  Serial.begin(9600); 
  delay(2000);    //第一次通电后延时2秒，目的为给一定时间模块初始化
}


void loop() 
{
  unsigned int dat;
  float db_value;

  dat = get_ad_value();
  db_value = (dat*120.0/1024.0*1.515);  //120是最大分贝值，1024是AD值，根据单片机需要修改，1.515，是AD参考电压为5V，而模块最大输出值是3.3，所以5/3.3=1.515，还原最大值。
  if(db_value < 50)
  {
    db_value += 40;   //DB小于50后，加上40，用于显示最小DB值，目的是模仿分贝仪最小值，无意义。
  }
  Serial.print("db:  ");
  Serial.println(db_value);

  delay(10);
}

unsigned int get_ad_value(void)
{
  unsigned char a,b,c;
  unsigned int temp;
  unsigned long int mindat=0,maxdat=0,sumdat=0;
  
  for(a=0;a<100;a++)
  {
    AD_TAB[a] = analogRead(A0);
    sumdat = sumdat + AD_TAB[a];
    delay(10);
  }

  for(b=0;b<100;b++)
  {
    for(c=0;c<10-b;c++)
    {
      if(AD_TAB[c] > AD_TAB[c+1])
      {
        temp = AD_TAB[c+1];
        AD_TAB[c+1] = AD_TAB[c];
        AD_TAB[c] = temp;
      }
    }
  }

  for(a=0;a<20;a++)
  {
    mindat = mindat + AD_TAB[a];
  }

  for(a=80;a<100;a++)
  {
    maxdat = maxdat + AD_TAB[a];
  }

  temp = (sumdat - mindat - maxdat)/60;

  return(temp);
}

