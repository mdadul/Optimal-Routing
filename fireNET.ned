simple fireNET
{
    parameters:
        @signal[arrival](type="long");
        @statistic[hopCount](title="hop count"; source="arrival"; record=vector,stats; interpolationmode=none);

        @display("i=block/routing");
        
        int SRC = default(0);
        int DEST = default(5);


    gates:
        inout gate[];  // declare two way connections
}

network OptimalRouting
{
    parameters:
        @figure[description](type=text; pos=5,20; font=,,bold; text="Random routing example - displaying last hop count");
        @figure[lasthopcount](type=text; pos=5,35; text="last hopCount: N/A");
    types:
        channel Channel extends ned.DelayChannel
        {
            delay = 100ms;
        }
    submodules:

        fireNET_Router[10]: fireNET {

            @display("i=block/app2");
        }
    connections:

        fireNET_Router[0].gate++ <--> Channel <--> fireNET_Router[1].gate++;
        fireNET_Router[0].gate++ <--> Channel <--> fireNET_Router[2].gate++;
        fireNET_Router[0].gate++ <--> Channel <--> fireNET_Router[7].gate++;
        fireNET_Router[1].gate++ <--> Channel <--> fireNET_Router[7].gate++;
        fireNET_Router[1].gate++ <--> Channel <--> fireNET_Router[3].gate++;
        fireNET_Router[2].gate++ <--> Channel <--> fireNET_Router[6].gate++;
        fireNET_Router[2].gate++ <--> Channel <--> fireNET_Router[8].gate++;
        fireNET_Router[3].gate++ <--> Channel <--> fireNET_Router[5].gate++;
        fireNET_Router[3].gate++ <--> Channel <--> fireNET_Router[4].gate++;
        fireNET_Router[4].gate++ <--> Channel <--> fireNET_Router[6].gate++;
        fireNET_Router[4].gate++ <--> Channel <--> fireNET_Router[8].gate++;
        fireNET_Router[5].gate++ <--> Channel <--> fireNET_Router[6].gate++;
        fireNET_Router[5].gate++ <--> Channel <--> fireNET_Router[9].gate++;
        fireNET_Router[7].gate++ <--> Channel <--> fireNET_Router[9].gate++;
        fireNET_Router[8].gate++ <--> Channel <--> fireNET_Router[9].gate++;
}
