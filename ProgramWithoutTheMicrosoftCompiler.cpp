// StandardOfData.cpp : Defines the entry point for the console application.
//


#include <iostream>
#include <string>
#include <algorithm>
#include <typeinfo>
#include <fstream>
#include <cstdio>
#include <cctype>
#include <ctime>
#include <sstream>
#include <exception>

#define DUMP "DUMP"
#define TEMP "TEMP"
#define SIZEOFTABLEINFOPEN 30 ///Protect from stackOverflow
#define TEXTSUFFIX ".txt"


namespace Suffix
{
void addOrNonAddSuffix(std::string &name)
{
    unsigned int whereFind = name.find(TEXTSUFFIX);

    if (whereFind != std::string::npos)
    {



    }
    else
    {
        name += TEXTSUFFIX;
    }
}
}
namespace FileOpenAndStreamNamespace
{
void saveFile(std::string nameOfFile,std::string textToSave)
{

    Suffix::addOrNonAddSuffix(nameOfFile);

    std::ofstream ff(nameOfFile,std::ios::app);

    ff << textToSave+'\n';

    ff.close();
}

}

class File
{

public:

    virtual void act(std::string nameOfFile) = 0;

};
class SaveToFile :public File
{

public:

    virtual void act(std::string nameOfFile);
    void act(std::string nameOfFile, std::string textToWrite);

};
class OpenFile :public File
{

public:
    virtual void act(std::string nameOfFile);


};
class DumpFile :public File
{

public:
    bool static checkDumpFile(std::string nameOfFile);
    virtual void act(std::string nameOfFile);
    static void copyRealFile(std::string nameOfFile, bool statusOfFile);

};
class SearchFileForBadSaved :public File
{

public:
    virtual void act(std::string nameOfFile);


};
class RealFile :public File
{
public:
    virtual void act(std::string nameOfFile);


};
class TestManipulateOfDate
{


public:
    static bool testValid(const unsigned int &firstHours, const unsigned int &firstMinutes, const unsigned int &secondHour, const unsigned int &secondMinutes, const unsigned int timeOfMinute, const unsigned int timeOfMidNight);

};


class ActualData
{

public:
    static std::string returnActualDate();

};
///Method
std::string ActualData::returnActualDate()
{


    struct tm *tim;
    time_t tt = time(&tt);

    tim = localtime(&tt);
    //asctime(&tm);



    //std::cout << "Today is: " << tm.tm_mday << " " << tm.tm_mon + 1 << " " << tm.tm_year + 1900 << '\n';

    //std::string StringToReturn = std::to_string(tm.tm_mday) + "-" + std::to_string(tm.tm_mon + 1) + "-" + std::to_string(tm.tm_year + 1900);

    std::stringstream tempMday;
    tempMday<<tim->tm_mday<<"-"<<tim->tm_mon + 1<<"-"<<tim->tm_year + 1900;
    std::string StringToReturn=tempMday.str();



    return StringToReturn;
}
bool TestManipulateOfDate::testValid(const unsigned int &firstHours, const unsigned int &firstMinutes, const unsigned int &secondHour, const unsigned int &secondMinutes, const unsigned int timeOfMinute = 60, const unsigned int timeOfMidNight = 24)
{
    if (firstHours >= timeOfMidNight || secondHour >= timeOfMidNight || firstMinutes >= timeOfMinute || secondMinutes >= timeOfMinute)
    {

        throw std::exception();
        return false;
        //exit(-1);
    }
    return true;
}

void SaveToFile::act(std::string nameOfFile)
{

    std::string tempCin;
    std::cout << "Insert your text: " << '\n';
    std::getline(std::cin, tempCin);
    FileOpenAndStreamNamespace::saveFile(nameOfFile, tempCin);
    /**
    std::ofstream ff(nameOfFile);

    ff << tempCin;

    ff.close();
    */
}
void SaveToFile::act(std::string nameOfFile, std::string textToWrite)
{

    FileOpenAndStreamNamespace::saveFile(nameOfFile,textToWrite);


}
void PROTECT_FROM_STACK_OVERFLOW(std::string);
void OpenFile::act(std::string nameOfFile)
{


    ///TEST
    Suffix::addOrNonAddSuffix(nameOfFile);
    PROTECT_FROM_STACK_OVERFLOW(nameOfFile);
    char buffer[SIZEOFTABLEINFOPEN];///This is dangerous and can simple stack overflow
    FILE * fp;
    fp = fopen(nameOfFile.c_str(), "r");
    if (!fp)
    {
        std::perror("Failed while opened file...");
        std::_Exit(EXIT_FAILURE);
    }


    int lineCounter = 0;
    while ((std::fgets(buffer,sizeof buffer,fp)!=NULL))
    {
        lineCounter++;
        std::cout << lineCounter <<". "<<buffer;


        std::string tempValue = std::string(buffer);

    }

    fclose(fp);



    std::string temp;

    std::ifstream ff(nameOfFile);
    while (std::getline(ff, temp))
    {
        //std::cout << temp << '\n';
    }


    ff.close();


}

void RealFile::act(std::string nameOfFile)
{
    DumpFile dumpFile;
    dumpFile.act(nameOfFile);
    ///Next

}
bool DumpFile::checkDumpFile(std::string nameOfFile)
{

    FILE *file;
    std::string tempToFOpen = DUMP + nameOfFile;
    file = fopen(tempToFOpen.c_str(), "r");
    if (file)
    {


        fclose(file);
        return true;
    }
    else return false;



    ///if Dump File exist, replace file with it

    return true;
}
void DumpFile::act(std::string nameOfFile)
{

    if (checkDumpFile(nameOfFile))
    {

        DumpFile::copyRealFile(nameOfFile, false);

        std::cout << "Restore file..." << '\n';
    }
    else
    {
        DumpFile::copyRealFile(nameOfFile, true);

    }

}
void DumpFile::copyRealFile(std::string nameOfFile, bool statusOfFile = true)
{
    std::string StringDump = DUMP + nameOfFile;
    std::string StringTemp = std::string("TEMP") + DUMP + nameOfFile;
    /*auto remFile = [](std::string& name) {

    	return [](std::string& name) {
    	std::remove(name.c_str());


    	};
    };*/





    if (std::remove((StringTemp.c_str())) == 0)
    {
        std::perror("Previous Running was bad");

    }
    if (statusOfFile)
    {
        std::ifstream src(nameOfFile, std::ios::binary);
        std::ofstream dst(StringTemp, std::ios::binary);
        dst << src.rdbuf();

        src.close();
        dst.close();
        std::rename(StringTemp.c_str(), StringDump.c_str());
    }
    else
    {

        std::cout << "Your save restore before last modification" << '\n';

        std::ifstream src(StringDump, std::ios::binary);
        std::ofstream dst(nameOfFile, std::ios::binary);
        dst << src.rdbuf();
        dst.close();

    }

    //    std::this_thread::sleep_for(std::chrono::seconds(15));
    if (std::remove((StringDump.c_str())) != 0)std::perror("Error while delete");

}


void SearchFileForBadSaved::act(std::string nameOfFile)
{

    if (DumpFile::checkDumpFile(nameOfFile))
    {
        DumpFile::copyRealFile(nameOfFile, false);
    }

}

///END OF METHOD
void redict(File *fp, std::string nameOfFile)
{
    PROTECT_FROM_STACK_OVERFLOW(nameOfFile);

    fp->act(nameOfFile);
}


class LoadData;
class ManipulateOfDate
{
    int hour
    , minutes;
public:
    void setHour(int h)
    {
        hour = h;
    }

    void setMinutes(int m)
    {
        minutes = m;
    }

    static void convertOfDate(int &hours, int &minutes, int position, const std::string &data);
    static void dealWithIt(int position, std::string &data, int &hours, int &minutes);
    static std::string countSpendTime(const unsigned int &hours, const unsigned int &minutes, const unsigned int &secondHour, const unsigned int &secondMinutes, const unsigned int timeOfMinute, const unsigned int timeOfMidNight);



    friend class LoadData;

};

class LoadData
{
    std::string firstPartOfDate;
    std::string secondPartOfDate;
public:

    LoadData():firstPartOfDate(""),secondPartOfDate("")
    {

    }

    std::string validOfDate(std::string &spendTime);
    std::string returnFirstPartOfDate()
    {
        return firstPartOfDate;
    }
    std::string returnSecondPartOfDate()
    {
        return secondPartOfDate;
    }
};

class RemoveDate
{
public:

    static void removeSpaceWithStringDate(std::string &textToRemoveString);
};
class ConvertStandardTime
{

public:
    static time_t changeToSecondsMinutes(int minutes);
    static time_t changeToSecondsHour(int hours);
    static void changeToHoursAndMinutes(time_t secondsHour, time_t secondsMinutes);

};
class Test
{
public:
    static  bool testOfInsertedData(std::string);

};

bool Test::testOfInsertedData(std::string test)
{
    for (auto n : test)
    {
        if ((int)(n)>59)
        {
            std::cout << "Try again!" << '\n';
            return false;
        }

    }
    return true;
}

void loadValue()
{

}
template<typename T, typename ...Rest>
void loadValue(T &d, Rest&...r)
{

    std::getline(std::cin, d);
    while (!Test::testOfInsertedData(d))loadValue(d);

    loadValue(r...);
    //return;
}


[[deprecated("Better idea is loadValue")]]
void testLoadValue(std::string &d)
{
    std::cin.clear();
    std::cin.sync();

    std::getline(std::cin, d);
}

[[deprecated("Can cause problem")]]
void RemoveDate::removeSpaceWithStringDate(std::string &textToRemoveString)
{

    textToRemoveString.erase(std::remove_if(textToRemoveString.begin(), textToRemoveString.end(), [](char x)
    {
        return std::isspace(x);
    }), textToRemoveString.end());

}


time_t ConvertStandardTime::changeToSecondsMinutes(int minutes)
{


    minutes *= 60;
    time_t secondsMinutes = minutes;



    return secondsMinutes;

}
time_t ConvertStandardTime::changeToSecondsHour(int hours)
{
    hours *= 3600;

    time_t secondsHour = hours;



    return secondsHour;

}
void ConvertStandardTime::changeToHoursAndMinutes(time_t secondsHour, time_t secondsMinutes)
{


    time_t hours = secondsHour / 3600;


    time_t minutes = secondsMinutes / 60;




}
int main();
void ManipulateOfDate::convertOfDate(int &hours, int &minutes, int position, const std::string &data)
{


    //charToHour[0]=;
    //  charToHour[1]=);
    std::string hourToString;
    hourToString.resize(3);

    hourToString[0] = data.at(0);
    hourToString[1] = data.at(1);

    // while(Test::testOfInsertedData(hourToString))loadValue(data);
    if ((int)hourToString[0]>57 || (int)hourToString[0]<48 ||
            (int)hourToString[1]>57 || (int)hourToString[1]<48
       )
    {
        std::cout << "Bad format of hours. Try again" << '\n';

        main();
        return;
    }

    hours = std::atoi(hourToString.c_str());

    std::string minuteToString;
    minuteToString.resize(3);

    minuteToString[0] = data.at(3);

    minuteToString[1] = data.at(4);




    if ((int)minuteToString[0]>57 || (int)minuteToString[0]<48 ||
            (int)minuteToString[1]>57 || (int)minuteToString[1]<48
       )
    {
        std::cout << "Bad format of minutes. Try again" << '\n';
        main();
        return;

    }

    minutes = std::atoi(minuteToString.c_str());

}
void ManipulateOfDate::dealWithIt(int position, std::string &data, int &hours, int &minutes)
{

    convertOfDate(hours, minutes, position, data);



    time_t secondsMinutes = ConvertStandardTime::changeToSecondsMinutes(minutes);
    time_t secondsHours = ConvertStandardTime::changeToSecondsHour(hours);

    ConvertStandardTime::changeToHoursAndMinutes(secondsHours, secondsMinutes);
}

std::string ManipulateOfDate::countSpendTime(const unsigned int &firstHours, const unsigned int &firstMinutes, const unsigned int &secondHour, const unsigned int &secondMinutes, const unsigned int timeOfMinute = 60, const unsigned int timeOfMidNight = 24)
{

    unsigned int hour
    , minute;

    try
    {

        TestManipulateOfDate::testValid(firstHours, firstMinutes, secondHour, secondMinutes, timeOfMinute, timeOfMidNight);
    }
    catch (std::exception &e)
    {
        std::cerr << "Error: "<<"Hours or minutes is higher than should be, or you write a negative number. Program will exit" << '\n';
        throw;
    }

    bool issetFirstValue = 0;
    if (secondHour<firstHours )
    {
        issetFirstValue = 1;
        hour = timeOfMidNight - firstHours + secondHour;

        minute = firstMinutes - secondMinutes;

        if (minute<0)minute = (minute*(-1));

        minute = timeOfMinute - minute;
    }
    if (secondHour == firstHours&& secondMinutes<firstMinutes)
    {

        issetFirstValue = 1;
        hour = timeOfMidNight - firstHours + secondHour-1;

        minute = firstMinutes - secondMinutes;

        if (minute<0)minute = (minute*(-1));

        minute = timeOfMinute - minute;

    }

    if (secondMinutes<firstMinutes)
    {


        if (!issetFirstValue)hour = secondHour - firstHours - 1;

        minute = firstMinutes - secondMinutes;

        if (minute<0)minute = (minute*(-1));



        minute = timeOfMinute - minute;

    }
    else
    {

        minute = secondMinutes - firstMinutes;
        if (!issetFirstValue)	hour = secondHour - firstHours;
    }
    if (hour<0)hour = (hour*(-1));
    if (minute == timeOfMinute)hour += 1;


    std::stringstream tempMday;
    tempMday<<hour<<" "<<minute;
    std::string StringToReturn=tempMday.str();


    return StringToReturn;
}


std::string LoadData::validOfDate(std::string &spendTime)//Spending Time will changing to spendingOfTime in format Hour Minutes
{
jump:///GOTO
    std::cout << "Insert date in format hours:minutes or hours minutes.\n Valid format: hh:min or hh min. Insert start.\nAfter it, insert end time, as the start" << '\n';

    loadValue(firstPartOfDate, secondPartOfDate);



    if (firstPartOfDate.length()>5 || firstPartOfDate.length() <= 4 || secondPartOfDate.length()>5 || secondPartOfDate.length() <= 4)
    {
        std::cout << "Value is too short, or too long.Try again..." << '\n';

        goto jump;///JUMP TO START OF FUNCTION

    }


    ///Space

    int position = 0;

    int firstDataHours = 0;
    int secondDataHours = 0;
    int firstDataminutes = 0;
    int secondDataminutes = 0;

    if (firstPartOfDate.find(':') != firstPartOfDate.npos&&secondPartOfDate.find(':') != secondPartOfDate.npos)
    {
        //RemoveDate::removeSpaceWithStringDate(firstPartOfDate);
        //RemoveDate::removeSpaceWithStringDate(secondPartOfDate);

        position = firstPartOfDate.find_first_of(':');
    }

    else if (firstPartOfDate.find_first_of(' ') != firstPartOfDate.npos&&secondPartOfDate.find(' ') != secondPartOfDate.npos)
    {

        position = firstPartOfDate.find_first_of(' ');
    }
    else
    {

        std::cout << "Unrecognized Separator.Try again..." << '\n';
        //loadValue(firstPartOfDate, secondPartOfDate);
        goto jump;///JUMP TO START OF FUNCTION
    }





    ManipulateOfDate::dealWithIt(position, firstPartOfDate, firstDataHours, firstDataminutes);

    ManipulateOfDate::dealWithIt(position, secondPartOfDate, secondDataHours, secondDataminutes);





//	std::string  =

    std::stringstream tempMday;
    tempMday<<firstDataHours << " " << firstDataminutes << " " << secondDataHours << " " << secondDataminutes;
    std::string fullFormatDataToReturn=tempMday.str();


    spendTime=ManipulateOfDate::countSpendTime(firstDataHours, firstDataminutes, secondDataHours, secondDataminutes, 60, 24);







    return firstPartOfDate;

    //return ;

}


void PROTECT_FROM_STACK_OVERFLOW(std::string fname)
{
    std::ifstream ff(fname);
    std::string checkStackOverflowLength="";
    int counterLine = 0;
    while (std::getline(ff, checkStackOverflowLength))
    {
        counterLine++;
        if (checkStackOverflowLength.length() > SIZEOFTABLEINFOPEN-1)
        {

            std::cout << "This can be dangerous and cause stack overflow\n Check your file, that don't have line with text length lengther than 29. Problem is in: " << counterLine<<"Line"<<'\n';
            std::cout << "Program will exit" << '\n';

            std::_Exit(EXIT_FAILURE);
        }
    }
    ff.close();
}

bool stillInserter()
{
    bool stillInterter = 0;
    std::cout << "Add another activity ? Press 1 to add. 0 do exit" << '\n';
    std::cin >> stillInterter;
    if (stillInterter == 0)return false;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.sync();
    return true;

}

int main()
{


    // countSpendTime(h,m,h2,m2,minutes,midNight);



    ///PROBLEM:
    ///After second running, firstPartOfData does not contain any value, but second yes Solved!
    ///What if the fail in create dump ? rename file after create Dump File  Solved!
    ///Bad count time if minutes in second is lower, than first Solved!
    while (true)
    {
        std::string spendTime;
        LoadData loadData;
        loadData.validOfDate(spendTime);
        //std::cout <<"First: "<< loadData.returnFirstPartOfDate() << '\n';
        //std::cout << loadData.returnSecondPartOfDate() << '\n';
        std::string fullStringText=spendTime+ " "+loadData.returnFirstPartOfDate() +" "+ loadData.returnSecondPartOfDate();

        std::string nameOfFile = ActualData::returnActualDate();


        Suffix::addOrNonAddSuffix(nameOfFile);//optional



        File *fp;
        SaveToFile saveToFile;
        OpenFile openFile;
        RealFile realFile;
        saveToFile.act(nameOfFile, fullStringText);

        //fp = &saveToFile;
        //fp->act(nameOfFile);
        fp = &realFile;
        auto n =[&]()
        {
            redict(fp,nameOfFile);
        };
        n();
        fp = &openFile;
        redict(fp,nameOfFile);


        if (!stillInserter())break;


    }


    return 0;
}
