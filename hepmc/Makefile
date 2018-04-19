CC=g++
#LD_FLAGS=-L/home/ppe/n/nwarrack/heplocal/pythia8230/lib -lpythia8 -L/home/ppe/n/nwarrack/heplocal/HepMC_2.06.09/lib -lHepMC -ldl
LD_PYTHIA_FLAGS=-L/home/ppe/n/nwarrack/heplocal/pythia8230/lib -lpythia8 -ldl
LD_HEPMC_FLAGS=-L/home/ppe/n/nwarrack/heplocal/HepMC_2.06.09/lib -lHepMC
INCLUDE_PYTHIA_FLAGS=-I/home/ppe/n/nwarrack/heplocal/pythia8230/include
INCLUDE_HEPMC_FLAGS=-I/home/ppe/n/nwarrack/heplocal/HepMC_2.06.09/include


# default target, dependency on $(PROG) target
all: hepmceventcounter hepmcreader

$(PROG): $(OBJS)
	$(CC) $< $(LD_PYTHIA_FLAGS) $(LD_HEPMC_FLAGS) -o $(PROG)

$(OBJS): mainAB.cc
	$(CC) -c $(INCLUDE_PYTHIA_FLAGS) $(INCLUDE_HEPMC_FLAGS) mainAB.cc

hepmceventcounter: hepmceventcounter.o
	$(CC) $< $(LD_HEPMC_FLAGS) -o hepmceventcounter

hepmceventcounter.o: hepmceventcounter.cc
	$(CC) -c $(INCLUDE_HEPMC_FLAGS) hepmceventcounter.cc

hepmcreader: hepmcreader.o
	$(CC) $< $(LD_HEPMC_FLAGS) -o hepmcreader

hepmcreader.o: hepmcreader.cc
	$(CC) -c $(INCLUDE_HEPMC_FLAGS) hepmcreader.cc

.PHONY: clean cleaner test
test:
	./mainAB
clean:
	rm ./*.o
	rm hepmcreader
	rm hepmceventcounter
cleaner:
	rm ./*.o