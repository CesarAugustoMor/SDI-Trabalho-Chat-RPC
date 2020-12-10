struct insPalavraParam {
	string userName<>;
	string msg<>;
};

struct retChat {
	string userName<>;
	string msg<>;
  long int index;
};

program PALAVRA_PROG {
  version PALAVRA_VERSION {
    int inspalavra(string) = 1;
    string retfrase(void) = 2;
  }
  = 1;

  version PALAVRA_VERSION2 {
    int inspalavra(insPalavraParam) = 1;
    retChat retfrase(long int) = 2;
  }
  = 2;
}
= 0x30009998;