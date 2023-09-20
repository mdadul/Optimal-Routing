#include <stdio.h>
#include <string.h>
#include <omnetpp.h>
#include "fireNETMSG_m.h"

using namespace omnetpp;

class fireNET : public cSimpleModule
{
private:
    simsignal_t arrivalSignal;
    std::vector<std::vector<int>> dijkstraTable; // Dijkstra's table to store shortest path information

protected:
    virtual fireNetmsg *generateMessage();
    virtual void forwardMessage(fireNetmsg *msg);
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void buildDijkstraTable(); // Function to build Dijkstra's table
};

Define_Module(fireNET);

void fireNET::initialize()
{
    arrivalSignal = registerSignal("arrival");
    buildDijkstraTable(); // Build the Dijkstra table during initialization

    // Module 0 sends the first message
    if (getIndex() == 0) {
        // Boot the process scheduling the initial message as a self-message.
        fireNetmsg *msg = generateMessage();
        scheduleAt(0.0, msg);
    }
}

void fireNET::handleMessage(cMessage *msg)
{
    fireNetmsg *ttmsg = check_and_cast<fireNetmsg *>(msg);

    if (ttmsg->getDestination() == getIndex()) {
        // Message arrived
        int hopcount = ttmsg->getHopCount();
        // send a signal
        emit(arrivalSignal, hopcount);

        if (hasGUI()) {
            char label[50];
            // Write last hop count to string
            sprintf(label, "last hopCount = %d", hopcount);
            // Get a pointer to figure
            cCanvas *canvas = getParentModule()->getCanvas();
            cTextFigure *textFigure = check_and_cast<cTextFigure*>(canvas->getFigure("lasthopcount"));
            // Update figure text
            textFigure->setText(label);
        }

        EV << "Message " << ttmsg << " arrived after " << hopcount << " hops.\n";
        bubble("ARRIVED, starting a new one!");

        delete ttmsg;

        // Generate another one.
        EV << "Generating another message: ";
        fireNetmsg *newmsg = generateMessage();
        EV << newmsg << endl;
        forwardMessage(newmsg);
    }
    else {
        // We need to forward the message.
        forwardMessage(ttmsg);
    }
}

fireNetmsg *fireNET::generateMessage()
{
    // Produce source and destination addresses.
    int src = par("SRC");
    int n = getVectorSize();
    int dest = par("DEST");
//    if (dest >= src)
//        dest++;

    char msgname[20];
    sprintf(msgname, "src-%d-dest-%d", src, dest);

    // Create message object and set source and destination field.
    fireNetmsg *msg = new fireNetmsg(msgname);
    msg->setSource(src);
    msg->setDestination(dest);
    return msg;
}

void fireNET::forwardMessage(fireNetmsg *msg)
{
    int next_hop = dijkstraTable[msg->getSource()][msg->getDestination()];

    // Increment hop count.
    msg->setHopCount(msg->getHopCount() + 1);

    EV << "Forwarding message " << msg << " on gate[" << next_hop << "]\n";
    send(msg, "gate$o", next_hop);
}

void fireNET::buildDijkstraTable()
{
    int numNodes = getVectorSize();
    dijkstraTable.resize(numNodes, std::vector<int>(numNodes, -1)); // Initialize the table with -1 (indicating no direct link)

    for (int i = 0; i < numNodes; i++) {
        for (int j = 0; j < numNodes; j++) {
            if (i != j) {
                dijkstraTable[i][j] = i;
            }
        }
    }
}
