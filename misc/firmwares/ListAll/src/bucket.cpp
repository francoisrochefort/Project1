
#include <mc.h>

Bucket::Bucket(const int id, char* name) : id(id), name(String(name))
{
}

int Bucket::getId()
{
    return id;
}

String Bucket::getName() 
{
    return name;
}

Bucket::~Bucket() 
{
}


/*
original

Note: le CURVE_PTS_QTY = 16

doc["BucketID"] = 1;// (calibData.BucketId);
            doc["GlobalFactor"] = 10000;// calibData.Global_CorrectionFactor;
            doc["MinAngle"] = 1200;// calibData.MinAngle_10x;
            doc["ResetAngle"] = 1300;// calibData.ResetAngle_10x;
            doc["AddAngle"] = 1400;// calibData.AddAngle_10x;
            doc["MaxAngle"] = 1500;// calibData.MaxAngle_10x;
            doc["X0RefWeight"] = 0;// calibData.Profile.Curve0_Weight_kg;
            doc["X1RefWeight"] = 3333;// calibData.Profile.CurveX1_Weight_kg;


            JsonArray CalibX0R_arr = doc.createNestedArray("CalibX0R");

            for (int pt = 0; pt < CURVE_PTS_QTY; pt++)
            {


                  CalibX0R_arr[pt]["T"] = calibData.Profile.Curve0_RisingData.TimeStamp_ms[pt];
                  CalibX0R_arr[pt]["A"] = calibData.Profile.Curve0_RisingData.Angles[pt];
                  CalibX0R_arr[pt]["AS"] = calibData.Profile.Curve0_RisingData.SpeedRef[pt];
                  CalibX0R_arr[pt]["P"] = calibData.Profile.Curve0_RisingData.Pressure[pt];

            }
            //-------------------------------------------------------------------------
            JsonArray CalibX0L_arr = doc.createNestedArray("CalibX0L");

            for (int p = 0; p < CURVE_PTS_QTY; p++)
            {

                  CalibX0L_arr[p]["T"] = calibData.Profile.Curve0_LoweringData.TimeStamp_ms[p];
                  CalibX0L_arr[p]["A"] = calibData.Profile.Curve0_LoweringData.Angles[p];
                  CalibX0L_arr[p]["AS"] = calibData.Profile.Curve0_LoweringData.SpeedRef[p];
                  CalibX0L_arr[p]["P"] = calibData.Profile.Curve0_LoweringData.Pressure[p];

            }

            JsonArray CalibX1R_arr = doc.createNestedArray("CalibX1R");

            for (int p = 0; p < CURVE_PTS_QTY; p++)
            {

                  CalibX1R_arr[p]["T"] = calibData.Profile.CurveX1_RisingData.TimeStamp_ms[p];
                  CalibX1R_arr[p]["A"] = calibData.Profile.CurveX1_RisingData.Angles[p];
                  CalibX1R_arr[p]["AS"] = calibData.Profile.CurveX1_RisingData.SpeedRef[p];
                  CalibX1R_arr[p]["P"] = calibData.Profile.CurveX1_RisingData.Pressure[p];

            }

            JsonArray CalibX1L_arr = doc.createNestedArray("CalibX1L");

            for (int p = 0; p < CURVE_PTS_QTY; p++)
            {

                  CalibX1L_arr[p]["T"] = calibData.Profile.CurveX1_LoweringData.TimeStamp_ms[p];
                  CalibX1L_arr[p]["A"] = calibData.Profile.CurveX1_LoweringData.Angles[p];
                  CalibX1L_arr[p]["AS"] = calibData.Profile.CurveX1_LoweringData.SpeedRef[p];
                  CalibX1L_arr[p]["P"] = calibData.Profile.CurveX1_LoweringData.Pressure[p];

            }

            //serializeJsonPretty(doc, jsbdata);
            cString jsStr;
            serializeJson(doc, jsStr);
            filehlp.WriteFile(jsStr.c_str());
            Serial.print(jsStr);
            doc.clear();
            

            
            for (int p = 0; p < CURVE_PTS_QTY; p++)
            {
                  doc["LowSpdFactorCrvX0"][p]["F"] = calibData.Profile.DynamicCurv0_Correction.LowSpeedFactor[p];
            }



            for (int p = 0; p < CURVE_PTS_QTY; p++)
            {
                  doc["HiSpdFactorCrvX0"][p]["F"] = calibData.Profile.DynamicCurv0_Correction.HiSpeedFactor[p];
            }


            for (int p = 0; p < CURVE_PTS_QTY; p++)
            {
                  doc["LowSpdFactorCrvX1"][p]["F"] = calibData.Profile.DynamicCurvX_Correction.LowSpeedFactor[p];
            }

            for (int p = 0; p < CURVE_PTS_QTY; p++)
            {
                  doc["HiSpdFactorCrvX1"][p]["F"] = calibData.Profile.DynamicCurvX_Correction.HiSpeedFactor[p];
            }




*/