unsigned float_negpwr2(int x) {
    int offset=0;

    if(x>149){
      return 0;
    }else if(x>126){
      offset=x-126;
      return (0x800000)>>offset;
    }else if(x>-128){
      return (127-x)<<23;
    }else{
      return 0x7f800000;
    }
    return 2;
}
