#include "datacollector.h"
#include <vector>

void DataCollector::recon()
{
    myo->setStreamEmg(myo::Myo::streamEmgEnabled);
}

DataCollector::DataCollector(int& er)

{
    emgSamples.resize(8);               // The values of this array is set by onEmgData() above.
    emgSamplesD.resize(8);               // The values of this array is set by onEmgData() above.
    accSamplesD.resize(4);
//    while(!myo)

    hub=new myo::Hub("ru.myCompany.MyoClass");

    myo = hub->waitForMyo(10000);
//    hub->
//    myo->unlock(myo->unlockHold);
//    myo->vibrate(myo->vibrationShort);
    myo->setStreamEmg(myo::Myo::streamEmgEnabled);

//    hub->addListener(this);
//hub->
}

void DataCollector::addListener()
{
    hub->addListener(this);
}

void DataCollector::removeListener()
{
    hub->removeListener(this);
}

void DataCollector::kick(int x)
{

 static int cnt=0;
 cnt++;
 if(cnt==100)
 {
     cnt=0;
//     hub->waitForMyo(1000);
//     myo->unlock(myo->unlockHold);
 //    myo->vibrate(myo->vibrationShort);
//     myo->setStreamEmg(myo::Myo::streamEmgEnabled);
 }
//    myo->unlock(myo->unlockHold);
    hub->run(10);
}
// onUnpair() is called whenever the Myo is disconnected from Myo Connect by the user.
//void DataCollector::onUnpair(myo::Myo* myo, uint64_t timestamp)
//{
//    // We've lost a Myo.        // Let's clean up some leftover state.
//    emgSamples.fill(0);
//}

// onEmgData() is called whenever a paired Myo has provided new EMG data, and EMG streaming is enabled.
void DataCollector::onEmgData(myo::Myo* myo, uint64_t timestamp, const int8_t* emg)
{
    //        static uint64_t timestampMin;
    //        static uint64_t timestampRelat;
    //        static uint64_t i=0;
    //        if(i==0)timestampMin=timestamp;
    //        i++;
    //        timestampRelat=timestamp-timestampMin;
    //        qDebug()<<i*1000000./timestampRelat;


    for (int i = 0; i < 8; i++) {
        emgSamples[i] = emg[i];
        emgSamplesD[i] = (float) emg[i];
    }
    emit qdc.EMG(emgSamplesD);
    //    qDebug()<<emgSamplesD[0];
}

// There are other virtual functions in DeviceListener that we could override here, like onAccelerometerData().
// For this example, the functions overridden above are sufficient.
void DataCollector::onAccelerometerData(myo::Myo* myo, uint64_t timestamp, const myo::Vector3<float> &accel)
{
    accSamplesD[0] = (double) accel.x();
    accSamplesD[1] = (double) accel.y();
    accSamplesD[2] = (double) accel.z();
    accSamplesD[3] = (double) accel.magnitude();
    //    qDebug()<<accSamplesD[0];
    //        hub->run(10);
}

void DataCollector::print()
{

    //                for (size_t i = 0; i < emgSamples.size(); i++)
    //                {
    //                    ostringstream oss;
    //                    oss << static_cast<int>(emgSamples[i]);
    //                    string emgString = oss.str();
    //                    QString qemgString = QString::fromStdString(emgString);
    //                    QString qbuffStr =  QString::fromStdString(string(4 - emgString.size(), ' '));
    //                    qDebug() << '[' << qemgString <<  qbuffStr << ']';
    //                }
}

//DataCollector::~DataCollector()
//{
//    delete myo;
//    qDebug() << '\r';
//}
