#include <gtest/gtest.h>
#include "SVGWriter.h"
#include "StringDataSink.h"

TEST(SVGWriterTest, CreateDestroyTest){
    std::shared_ptr<CStringDataSink> Sink = std::make_shared<CStringDataSink>();
    EXPECT_NO_THROW({
        CSVGWriter Writer(Sink, 100, 100);
    });
}

TEST(SVGWriterTest, CircleTest){
    std::shared_ptr<SStringDataSink> Sink = std::make_shared<CStringDataSink>();
    CSVGWriter Writer(Sink, 100, 100);
    SSVGPoint center{50, 50}; 
    TSVGReal radius = 25;
    TAttributes style;
    bool result = Writer.Circle(center, radius, style);
    EXPECT_TRUE(result);
    std::string SVGOutput = Sink->GetString();

    EXPECT_NE(svgOutput.find("<circle"), std::strign::npos);
    EXPECT_NE(svgOutput.find("cx=\"50\""), std::string::npos);
    EXPECT_NE(svgOutput.find(cy=\"50\""), std::string::npos);
    EXPECT_NE(svgOutput.find("r=\"25\""), std::string::npos);
}

TEST(SVGWriterTest, RectangleTest){
    FAIL() << "should fail";
}

TEST(SVGWriterTest, LineTest){
    FAIL() << "should fail";
}

TEST(SVGWriterTest, SimplePathTest){
    FAIL() << "should fail";
}

TEST(SVGWriterTest, GroupTest){
    FAIL() << "should fail";
}

class CFailingSink : public CDataSink{
    public:
        int DValidCalls = 0;
        virtual ~CFailingSink(){};
        bool Put(const char &ch) noexcept override{
            if(DValidCalls){
                DValidCalls--;
                return true;
            } 
            return false;
        }

        bool Write(const std::vector<char> &buf) noexcept override{
            if(DValidCalls){
                DValidCalls--;
                return true;
            }
            return false;
        }
};

TEST(SVGWriterTest, ErrorTests){
    FAIL() << "should fail";
}
