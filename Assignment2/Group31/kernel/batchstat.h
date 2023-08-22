struct batchstat {
  int exectime;             //Batch exectution time
  int stime;                //Batch start time
  int endtime;              //Batch end time
  int bsize;                //Batch size
  int finsihed;             //Number of batch processes finished
  int turnaroundtime;       //Cumulative turnaround time
  int waitingtime;          //Cumulative waiting time
  int cumulativectime;      //Cumulative completion time
  int maxctime;             //Maximum completion time
  int minctime;             //Minimum completion time
  int nbursts;              //Number of CPU bursts for the batch
  int totalblength;         //Cumulative burst length
  int maxblength;           //Maximum burst length;
  int minblength;           //Minimum burst length
  int enbursts;             //Estimated Number of CPU bursts for the batch
  int etotalblength;        //Estimated Cumulative burst length
  int emaxblength;          //Estimated Maximum burst length;
  int eminblength;          //Estimated Minimum burst length
  int nerrorblength;        //Number of estimation errors
  int eberror;              //Cumulative burst estimation error
};
