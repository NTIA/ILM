#include <gtest/gtest.h>
#include "ilm.h"        // Include your shared library header
#include "Enums.h"      // For polarization constants
#include "Errors.h"     // For error constants
#include "Warnings.h"   // For warning constants

 #define APPROVALS_GOOGLETEST
#include "ApprovalTests.v.10.13.0.hpp"  // ApprovalTests framework

#include <fstream>      // For file I/O
#include <sstream>      // For string stream
#include <string>       // For string operations
#include <vector>       // For vector container
#include <iostream>     // For console output
#include <algorithm>    // For min/max functions
#include <iomanip>      // For formatting output

// Example test case
TEST(ILMTest, Test_ILM_Version) 
{
    EXPECT_STREQ(version(), "0.0.0");
}

// ValidateInputs test fixture
class ValidateInputsTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Initialize with valid default values
        h_tx = 50.0;    // meters
        h_rx = 10.0;    // meters
        p = 50.0;       // percentage (0-100)
        f_mhz = 1000.0; // MHz
        pol = POLARIZATION__HORIZONTAL;
        epsilon = 15.0; // relative permittivity
        sigma = 0.005;  // conductivity
        warnings = 0;   // clear warnings
    }

    double h_tx, h_rx, p, f_mhz, epsilon, sigma;
    int pol;
    long warnings;
};

// Test successful validation with default valid parameters
TEST_F(ValidateInputsTest, ValidInputs_Success) {
    int result = ValidateInputs(h_tx, h_rx, p, f_mhz, pol, epsilon, sigma, &warnings);
    
    EXPECT_EQ(result, SUCCESS);
    EXPECT_EQ(warnings, NO_WARNINGS);
}

// TX Terminal Height Tests
TEST_F(ValidateInputsTest, TxHeight_ValidRange_NoWarning) {
    h_tx = 500.0;  // Well within valid range
    int result = ValidateInputs(h_tx, h_rx, p, f_mhz, pol, epsilon, sigma, &warnings);
    
    EXPECT_EQ(result, SUCCESS);
    EXPECT_EQ(warnings & WARN__TX_TERMINAL_HEIGHT, 0);
}

TEST_F(ValidateInputsTest, TxHeight_NearLowerLimit_Warning) {
    h_tx = 0.9;  // Below 1.0 but above 0.5
    int result = ValidateInputs(h_tx, h_rx, p, f_mhz, pol, epsilon, sigma, &warnings);
    
    EXPECT_EQ(result, SUCCESS);
    EXPECT_NE(warnings & WARN__TX_TERMINAL_HEIGHT, 0);
}

TEST_F(ValidateInputsTest, TxHeight_NearUpperLimit_Warning) {
    h_tx = 1001.0;  // Above 1000.0 but below 3000.0
    int result = ValidateInputs(h_tx, h_rx, p, f_mhz, pol, epsilon, sigma, &warnings);
    
    EXPECT_EQ(result, SUCCESS);
    EXPECT_NE(warnings & WARN__TX_TERMINAL_HEIGHT, 0);
}

TEST_F(ValidateInputsTest, TxHeight_TooLow_Error) {
    h_tx = 0.4;  // Below 0.5
    int result = ValidateInputs(h_tx, h_rx, p, f_mhz, pol, epsilon, sigma, &warnings);
    
    EXPECT_EQ(result, ERROR__TX_TERMINAL_HEIGHT);
}

TEST_F(ValidateInputsTest, TxHeight_TooHigh_Error) {
    h_tx = 3001.0;  // Above 3000.0
    int result = ValidateInputs(h_tx, h_rx, p, f_mhz, pol, epsilon, sigma, &warnings);
    
    EXPECT_EQ(result, ERROR__TX_TERMINAL_HEIGHT);
}

// RX Terminal Height Tests
TEST_F(ValidateInputsTest, RxHeight_ValidRange_NoWarning) {
    h_rx = 500.0;  // Well within valid range
    int result = ValidateInputs(h_tx, h_rx, p, f_mhz, pol, epsilon, sigma, &warnings);
    
    EXPECT_EQ(result, SUCCESS);
    EXPECT_EQ(warnings & WARN__RX_TERMINAL_HEIGHT, 0);
}

TEST_F(ValidateInputsTest, RxHeight_NearLowerLimit_Warning) {
    h_rx = 0.9;  // Below 1.0 but above 0.5
    int result = ValidateInputs(h_tx, h_rx, p, f_mhz, pol, epsilon, sigma, &warnings);
    
    EXPECT_EQ(result, SUCCESS);
    EXPECT_NE(warnings & WARN__RX_TERMINAL_HEIGHT, 0);
}

TEST_F(ValidateInputsTest, RxHeight_NearUpperLimit_Warning) {
    h_rx = 1001.0;  // Above 1000.0 but below 3000.0
    int result = ValidateInputs(h_tx, h_rx, p, f_mhz, pol, epsilon, sigma, &warnings);
    
    EXPECT_EQ(result, SUCCESS);
    EXPECT_NE(warnings & WARN__RX_TERMINAL_HEIGHT, 0);
}

TEST_F(ValidateInputsTest, RxHeight_TooLow_Error) {
    h_rx = 0.4;  // Below 0.5
    int result = ValidateInputs(h_tx, h_rx, p, f_mhz, pol, epsilon, sigma, &warnings);
    
    EXPECT_EQ(result, ERROR__RX_TERMINAL_HEIGHT);
}

TEST_F(ValidateInputsTest, RxHeight_TooHigh_Error) {
    h_rx = 3001.0;  // Above 3000.0
    int result = ValidateInputs(h_tx, h_rx, p, f_mhz, pol, epsilon, sigma, &warnings);
    
    EXPECT_EQ(result, ERROR__RX_TERMINAL_HEIGHT);
}

// Frequency Tests
TEST_F(ValidateInputsTest, Frequency_ValidRange_NoWarning) {
    f_mhz = 5000.0;  // Well within valid range
    int result = ValidateInputs(h_tx, h_rx, p, f_mhz, pol, epsilon, sigma, &warnings);
    
    EXPECT_EQ(result, SUCCESS);
    EXPECT_EQ(warnings & WARN__FREQUENCY, 0);
}

TEST_F(ValidateInputsTest, Frequency_NearLowerLimit_Warning) {
    f_mhz = 30.0;  // Below 40.0 but above 20.0
    int result = ValidateInputs(h_tx, h_rx, p, f_mhz, pol, epsilon, sigma, &warnings);
    
    EXPECT_EQ(result, SUCCESS);
    EXPECT_NE(warnings & WARN__FREQUENCY, 0);
}

TEST_F(ValidateInputsTest, Frequency_NearUpperLimit_Warning) {
    f_mhz = 15000.0;  // Above 10000.0 but below 20000.0
    int result = ValidateInputs(h_tx, h_rx, p, f_mhz, pol, epsilon, sigma, &warnings);
    
    EXPECT_EQ(result, SUCCESS);
    EXPECT_NE(warnings & WARN__FREQUENCY, 0);
}

TEST_F(ValidateInputsTest, Frequency_TooLow_Error) {
    f_mhz = 15.0;  // Below 20.0
    int result = ValidateInputs(h_tx, h_rx, p, f_mhz, pol, epsilon, sigma, &warnings);
    
    EXPECT_EQ(result, ERROR__FREQUENCY);
}

TEST_F(ValidateInputsTest, Frequency_TooHigh_Error) {
    f_mhz = 25000.0;  // Above 20000.0
    int result = ValidateInputs(h_tx, h_rx, p, f_mhz, pol, epsilon, sigma, &warnings);
    
    EXPECT_EQ(result, ERROR__FREQUENCY);
}

// Polarization Tests
TEST_F(ValidateInputsTest, Polarization_Horizontal_Valid) {
    pol = POLARIZATION__HORIZONTAL;
    int result = ValidateInputs(h_tx, h_rx, p, f_mhz, pol, epsilon, sigma, &warnings);
    
    EXPECT_EQ(result, SUCCESS);
}

TEST_F(ValidateInputsTest, Polarization_Vertical_Valid) {
    pol = POLARIZATION__VERTICAL;
    int result = ValidateInputs(h_tx, h_rx, p, f_mhz, pol, epsilon, sigma, &warnings);
    
    EXPECT_EQ(result, SUCCESS);
}

TEST_F(ValidateInputsTest, Polarization_Invalid_Error) {
    pol = 99;  // Invalid polarization value
    int result = ValidateInputs(h_tx, h_rx, p, f_mhz, pol, epsilon, sigma, &warnings);
    
    EXPECT_EQ(result, ERROR__POLARIZATION);
}

// Epsilon Tests
TEST_F(ValidateInputsTest, Epsilon_Valid_Success) {
    epsilon = 10.0;  // Valid value >= 1.0
    int result = ValidateInputs(h_tx, h_rx, p, f_mhz, pol, epsilon, sigma, &warnings);
    
    EXPECT_EQ(result, SUCCESS);
}

TEST_F(ValidateInputsTest, Epsilon_MinimumValid_Success) {
    epsilon = 1.0;  // Minimum valid value
    int result = ValidateInputs(h_tx, h_rx, p, f_mhz, pol, epsilon, sigma, &warnings);
    
    EXPECT_EQ(result, SUCCESS);
}

TEST_F(ValidateInputsTest, Epsilon_TooLow_Error) {
    epsilon = 0.9;  // Below 1.0
    int result = ValidateInputs(h_tx, h_rx, p, f_mhz, pol, epsilon, sigma, &warnings);
    
    EXPECT_EQ(result, ERROR__EPSILON);
}

// Sigma Tests
TEST_F(ValidateInputsTest, Sigma_Valid_Success) {
    sigma = 0.01;  // Valid positive value
    int result = ValidateInputs(h_tx, h_rx, p, f_mhz, pol, epsilon, sigma, &warnings);
    
    EXPECT_EQ(result, SUCCESS);
}

TEST_F(ValidateInputsTest, Sigma_VerySmall_Success) {
    sigma = 0.0001;  // Very small but positive
    int result = ValidateInputs(h_tx, h_rx, p, f_mhz, pol, epsilon, sigma, &warnings);
    
    EXPECT_EQ(result, SUCCESS);
}

TEST_F(ValidateInputsTest, Sigma_Zero_Error) {
    sigma = 0.0;  // Exactly zero
    int result = ValidateInputs(h_tx, h_rx, p, f_mhz, pol, epsilon, sigma, &warnings);
    
    EXPECT_EQ(result, ERROR__SIGMA);
}

TEST_F(ValidateInputsTest, Sigma_Negative_Error) {
    sigma = -0.001;  // Negative value
    int result = ValidateInputs(h_tx, h_rx, p, f_mhz, pol, epsilon, sigma, &warnings);
    
    EXPECT_EQ(result, ERROR__SIGMA);
}

// Percentage Tests
TEST_F(ValidateInputsTest, Percentage_Valid_Success) {
    p = 50.0;  // Valid percentage
    int result = ValidateInputs(h_tx, h_rx, p, f_mhz, pol, epsilon, sigma, &warnings);
    
    EXPECT_EQ(result, SUCCESS);
}

TEST_F(ValidateInputsTest, Percentage_NearLowerBound_Success) {
    p = 0.1;  // Just above 0
    int result = ValidateInputs(h_tx, h_rx, p, f_mhz, pol, epsilon, sigma, &warnings);
    
    EXPECT_EQ(result, SUCCESS);
}

TEST_F(ValidateInputsTest, Percentage_NearUpperBound_Success) {
    p = 99.9;  // Just below 100
    int result = ValidateInputs(h_tx, h_rx, p, f_mhz, pol, epsilon, sigma, &warnings);
    
    EXPECT_EQ(result, SUCCESS);
}

TEST_F(ValidateInputsTest, Percentage_Zero_Error) {
    p = 0.0;  // Exactly zero
    int result = ValidateInputs(h_tx, h_rx, p, f_mhz, pol, epsilon, sigma, &warnings);
    
    EXPECT_EQ(result, ERROR__INVALID_PERCENTAGE);
}

TEST_F(ValidateInputsTest, Percentage_Negative_Error) {
    p = -1.0;  // Negative percentage
    int result = ValidateInputs(h_tx, h_rx, p, f_mhz, pol, epsilon, sigma, &warnings);
    
    EXPECT_EQ(result, ERROR__INVALID_PERCENTAGE);
}

TEST_F(ValidateInputsTest, Percentage_Hundred_Error) {
    p = 100.0;  // Exactly 100
    int result = ValidateInputs(h_tx, h_rx, p, f_mhz, pol, epsilon, sigma, &warnings);
    
    EXPECT_EQ(result, ERROR__INVALID_PERCENTAGE);
}

TEST_F(ValidateInputsTest, Percentage_OverHundred_Error) {
    p = 101.0;  // Over 100
    int result = ValidateInputs(h_tx, h_rx, p, f_mhz, pol, epsilon, sigma, &warnings);
    
    EXPECT_EQ(result, ERROR__INVALID_PERCENTAGE);
}

// Multiple Warning Tests
TEST_F(ValidateInputsTest, MultipleWarnings_Success) {
    h_tx = 0.8;     // Warning: below 1.0
    h_rx = 1200.0;  // Warning: above 1000.0
    f_mhz = 35.0;   // Warning: below 40.0
    
    int result = ValidateInputs(h_tx, h_rx, p, f_mhz, pol, epsilon, sigma, &warnings);
    
    EXPECT_EQ(result, SUCCESS);
    EXPECT_NE(warnings & WARN__TX_TERMINAL_HEIGHT, 0);
    EXPECT_NE(warnings & WARN__RX_TERMINAL_HEIGHT, 0);
    EXPECT_NE(warnings & WARN__FREQUENCY, 0);
}

// Edge Cases
TEST_F(ValidateInputsTest, BoundaryValues_Success) {
    // Test exact boundary values that should succeed
    h_tx = 1.0;     // Exact lower warning boundary
    h_rx = 1000.0;  // Exact upper warning boundary  
    f_mhz = 40.0;   // Exact lower warning boundary
    
    int result = ValidateInputs(h_tx, h_rx, p, f_mhz, pol, epsilon, sigma, &warnings);
    
    EXPECT_EQ(result, SUCCESS);
    // These should not generate warnings since they're at the boundary
    EXPECT_EQ(warnings & WARN__TX_TERMINAL_HEIGHT, 0);
    EXPECT_EQ(warnings & WARN__RX_TERMINAL_HEIGHT, 0);
    EXPECT_EQ(warnings & WARN__FREQUENCY, 0);
}

// ===================== PointToPoint_Ex Tests =====================

// Helper structure to hold test parameters from CSV
struct P2PTestCase {
    double h_tx__meter;
    double h_rx__meter;
    double f__mhz;
    int pol;
    double epsilon;
    double sigma;
    double p;
};

// Helper structure to hold PFL data
struct PFLData {
    std::vector<double> pfl;
    bool isValid;
};

// Helper functions for reading CSV files
class CSVReader {
public:
    static std::vector<P2PTestCase> readP2PTestCases(const std::string& filename) {
        std::vector<P2PTestCase> testCases;
        std::ifstream file(filename);
        std::string line;
        
        // Skip header line
        if (std::getline(file, line)) {
            while (std::getline(file, line)) {
                if (line.empty()) continue;
                
                P2PTestCase testCase;
                std::stringstream ss(line);
                std::string cell;
                
                // Parse CSV: h_tx__meter,h_rx__meter,f__mhz,pol,epsilon,sigma,p
                if (std::getline(ss, cell, ',')) testCase.h_tx__meter = std::stod(cell);
                if (std::getline(ss, cell, ',')) testCase.h_rx__meter = std::stod(cell);
                if (std::getline(ss, cell, ',')) testCase.f__mhz = std::stod(cell);
                if (std::getline(ss, cell, ',')) testCase.pol = std::stoi(cell);
                if (std::getline(ss, cell, ',')) testCase.epsilon = std::stod(cell);
                if (std::getline(ss, cell, ',')) testCase.sigma = std::stod(cell);
                if (std::getline(ss, cell, ',')) testCase.p = std::stod(cell);
                
                testCases.push_back(testCase);
            }
        }
        file.close();
        return testCases;
    }
    
    static std::vector<PFLData> readPFLData(const std::string& filename) {
        std::vector<PFLData> pflDataList;
        std::ifstream file(filename);
        std::string line;
        
        while (std::getline(file, line)) {
            if (line.empty()) continue;
            
            PFLData pflData;
            pflData.isValid = true;
            
            std::stringstream ss(line);
            std::string cell;
            
            // First two numbers are special: number of points and distance increment
            if (std::getline(ss, cell, ',')) {
                double numPoints = std::stod(cell);
                pflData.pfl.push_back(numPoints);
            } else {
                pflData.isValid = false;
                continue;
            }
            
            if (std::getline(ss, cell, ',')) {
                double increment = std::stod(cell);
                pflData.pfl.push_back(increment);
            } else {
                pflData.isValid = false;
                continue;
            }
            
            // Read all elevation points
            while (std::getline(ss, cell, ',')) {
                if (!cell.empty()) {
                    pflData.pfl.push_back(std::stod(cell));
                }
            }
            
            pflDataList.push_back(pflData);
        }
        file.close();
        return pflDataList;
    }
};

// PointToPoint_Ex test fixture
class PointToPointExTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Get paths to test data files - try multiple possible locations
        std::vector<std::string> possiblePaths = {
            "tests/",           // Relative to build directory
            "../tests/",        // One level up from build
            "../../tests/",     // Two levels up from build/Debug
            "./tests/"          // Current directory
        };
        
        bool foundData = false;
        for (const auto& path : possiblePaths) {
            testCases = CSVReader::readP2PTestCases(path + "p2p.csv");
            pflDataList = CSVReader::readPFLData(path + "pfls.csv");
            
            if (!testCases.empty() && !pflDataList.empty()) {
                testDataPath = path;
                foundData = true;
                break;
            }
        }
        
        // Verify we loaded data successfully
        ASSERT_TRUE(foundData) << "Failed to load test data from any of the expected locations";
        ASSERT_FALSE(testCases.empty()) << "Failed to load test cases from p2p.csv";
        ASSERT_FALSE(pflDataList.empty()) << "Failed to load PFL data from pfls.csv";
        
        std::cout << "Loaded " << testCases.size() << " test cases and " 
                  << pflDataList.size() << " PFL datasets from " << testDataPath << std::endl;
    }
    
    std::string testDataPath;
    std::vector<P2PTestCase> testCases;
    std::vector<PFLData> pflDataList;
};

// Test basic functionality with first test case
TEST_F(PointToPointExTest, BasicFunctionality_FirstTestCase) {
    ASSERT_FALSE(testCases.empty());
    ASSERT_FALSE(pflDataList.empty());
    ASSERT_TRUE(pflDataList[0].isValid);
    
    const P2PTestCase& testCase = testCases[0];
    const PFLData& pflData = pflDataList[0];
    
    double A_db = 0.0;
    long warnings = 0;
    IntermediateValues interValues;
    
    int result = PointToPoint_Ex(
        testCase.h_tx__meter,
        testCase.h_rx__meter,
        const_cast<double*>(pflData.pfl.data()),
        testCase.f__mhz,
        testCase.pol,
        testCase.epsilon,
        testCase.sigma,
        testCase.p,
        &A_db,
        &warnings,
        &interValues
    );
    
    // Basic validation
    EXPECT_TRUE(result == SUCCESS || result == SUCCESS_WITH_WARNINGS);
    EXPECT_GT(A_db, 0.0) << "Transmission loss should be positive";
    EXPECT_LT(A_db, 500.0) << "Transmission loss should be reasonable (< 500 dB)";
    
    // Check intermediate values are populated
    EXPECT_GT(interValues.d__km, 0.0) << "Distance should be positive";
    EXPECT_GT(interValues.A_fs__db, 0.0) << "Free space loss should be positive";
    EXPECT_GE(interValues.A_ref__db, 0.0) << "Reference attenuation should be non-negative";
    
    // Validate horizon data
    EXPECT_GE(interValues.d_hzn__meter[0], 0.0) << "TX horizon distance should be non-negative";
    EXPECT_GE(interValues.d_hzn__meter[1], 0.0) << "RX horizon distance should be non-negative";
    EXPECT_GE(interValues.h_e__meter[0], 0.0) << "TX effective height should be non-negative";
    EXPECT_GE(interValues.h_e__meter[1], 0.0) << "RX effective height should be non-negative";
}

// Test all provided test cases
TEST_F(PointToPointExTest, AllTestCases_ValidResults) {
    // Ensure we have matching test cases and PFL data
    size_t maxTests = std::min(testCases.size(), pflDataList.size());
    ASSERT_GT(maxTests, 0) << "No valid test cases found";
    
    for (size_t i = 0; i < maxTests; ++i) {
        const P2PTestCase& testCase = testCases[i];
        const PFLData& pflData = pflDataList[i];
        
        // Skip invalid PFL data
        if (!pflData.isValid) {
            GTEST_SKIP() << "Skipping test case " << i << " due to invalid PFL data";
            continue;
        }
        
        double A_db = 0.0;
        long warnings = 0;
        IntermediateValues interValues;
        
        int result = PointToPoint_Ex(
            testCase.h_tx__meter,
            testCase.h_rx__meter,
            const_cast<double*>(pflData.pfl.data()),
            testCase.f__mhz,
            testCase.pol,
            testCase.epsilon,
            testCase.sigma,
            testCase.p,
            &A_db,
            &warnings,
            &interValues
        );
        
        // Test should succeed or succeed with warnings
        EXPECT_TRUE(result == SUCCESS || result == SUCCESS_WITH_WARNINGS) 
            << "Test case " << i << " failed with error code: " << result;
        
        // Basic sanity checks on results
        EXPECT_GT(A_db, 0.0) << "Test case " << i << ": Transmission loss should be positive";
        EXPECT_LT(A_db, 1000.0) << "Test case " << i << ": Transmission loss should be reasonable";
        EXPECT_GT(interValues.d__km, 0.0) << "Test case " << i << ": Distance should be positive";
        
        // Log results for debugging
        std::cout << "Test case " << i << ": "
                  << "A_db=" << A_db 
                  << ", d_km=" << interValues.d__km
                  << ", warnings=0x" << std::hex << warnings << std::dec
                  << ", mode=" << interValues.mode << std::endl;
    }
}

// Test error conditions with invalid inputs
TEST_F(PointToPointExTest, InvalidInputs_ErrorHandling) {
    ASSERT_FALSE(pflDataList.empty());
    ASSERT_TRUE(pflDataList[0].isValid);
    
    const PFLData& pflData = pflDataList[0];
    double A_db = 0.0;
    long warnings = 0;
    IntermediateValues interValues;
    
    // Test with invalid frequency (too low)
    int result = PointToPoint_Ex(
        10.0,   // h_tx
        2.0,    // h_rx
        const_cast<double*>(pflData.pfl.data()),
        15.0,   // f_mhz - too low (should be >= 20)
        POLARIZATION__HORIZONTAL,
        15.0,   // epsilon
        0.008,  // sigma
        50.0,   // p
        &A_db,
        &warnings,
        &interValues
    );
    
    EXPECT_EQ(result, ERROR__FREQUENCY);
    
    // Test with invalid polarization
    result = PointToPoint_Ex(
        10.0,   // h_tx
        2.0,    // h_rx
        const_cast<double*>(pflData.pfl.data()),
        1000.0, // f_mhz
        99,     // pol - invalid
        15.0,   // epsilon
        0.008,  // sigma
        50.0,   // p
        &A_db,
        &warnings,
        &interValues
    );
    
    EXPECT_EQ(result, ERROR__POLARIZATION);
    
    // Test with invalid percentage
    result = PointToPoint_Ex(
        10.0,   // h_tx
        2.0,    // h_rx
        const_cast<double*>(pflData.pfl.data()),
        1000.0, // f_mhz
        POLARIZATION__HORIZONTAL,
        15.0,   // epsilon
        0.008,  // sigma
        150.0,  // p - too high (should be < 100)
        &A_db,
        &warnings,
        &interValues
    );
    
    EXPECT_EQ(result, ERROR__INVALID_PERCENTAGE);
}

// Test warning conditions
TEST_F(PointToPointExTest, WarningConditions_SuccessWithWarnings) {
    ASSERT_FALSE(pflDataList.empty());
    ASSERT_TRUE(pflDataList[0].isValid);
    
    const PFLData& pflData = pflDataList[0];
    double A_db = 0.0;
    long warnings = 0;
    IntermediateValues interValues;
    
    // Test with parameters that should generate warnings
    int result = PointToPoint_Ex(
        0.8,    // h_tx - below warning threshold (< 1.0)
        1200.0, // h_rx - above warning threshold (> 1000.0)
        const_cast<double*>(pflData.pfl.data()),
        35.0,   // f_mhz - below warning threshold (< 40.0)
        POLARIZATION__VERTICAL,
        15.0,   // epsilon
        0.008,  // sigma
        50.0,   // p
        &A_db,
        &warnings,
        &interValues
    );
    
    EXPECT_EQ(result, SUCCESS_WITH_WARNINGS);
    EXPECT_NE(warnings & WARN__TX_TERMINAL_HEIGHT, 0);
    EXPECT_NE(warnings & WARN__RX_TERMINAL_HEIGHT, 0);
    EXPECT_NE(warnings & WARN__FREQUENCY, 0);
    EXPECT_GT(A_db, 0.0);
}

// Test different polarizations
TEST_F(PointToPointExTest, DifferentPolarizations_ValidResults) {
    ASSERT_FALSE(testCases.empty());
    ASSERT_FALSE(pflDataList.empty());
    ASSERT_TRUE(pflDataList[0].isValid);
    
    const P2PTestCase& baseCase = testCases[0];
    const PFLData& pflData = pflDataList[0];
    
    // Test horizontal polarization
    double A_db_horizontal = 0.0;
    long warnings_h = 0;
    IntermediateValues interValues_h;
    
    int result_h = PointToPoint_Ex(
        baseCase.h_tx__meter,
        baseCase.h_rx__meter,
        const_cast<double*>(pflData.pfl.data()),
        baseCase.f__mhz,
        POLARIZATION__HORIZONTAL,
        baseCase.epsilon,
        baseCase.sigma,
        baseCase.p,
        &A_db_horizontal,
        &warnings_h,
        &interValues_h
    );
    
    // Test vertical polarization
    double A_db_vertical = 0.0;
    long warnings_v = 0;
    IntermediateValues interValues_v;
    
    int result_v = PointToPoint_Ex(
        baseCase.h_tx__meter,
        baseCase.h_rx__meter,
        const_cast<double*>(pflData.pfl.data()),
        baseCase.f__mhz,
        POLARIZATION__VERTICAL,
        baseCase.epsilon,
        baseCase.sigma,
        baseCase.p,
        &A_db_vertical,
        &warnings_v,
        &interValues_v
    );
    
    // Both should succeed
    EXPECT_TRUE(result_h == SUCCESS || result_h == SUCCESS_WITH_WARNINGS);
    EXPECT_TRUE(result_v == SUCCESS || result_v == SUCCESS_WITH_WARNINGS);
    
    // Both should produce valid results
    EXPECT_GT(A_db_horizontal, 0.0);
    EXPECT_GT(A_db_vertical, 0.0);
    
    // Results might differ due to polarization effects
    std::cout << "Horizontal polarization: A_db = " << A_db_horizontal 
              << ", Vertical polarization: A_db = " << A_db_vertical << std::endl;
}

// ===================== Area_Ex Tests =====================

// Helper structure to hold area test parameters from CSV
struct AreaTestCase {
    double h_tx__meter;
    double h_rx__meter;
    int tx_siting_criteria;
    int rx_siting_criteria;
    double d__km;
    double delta_h__meter;
    double f__mhz;
    int pol;
    double epsilon;
    double sigma;
    double p;
};

// Area_Ex test fixture
class AreaExTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Get paths to test data files - try multiple possible locations
        std::vector<std::string> possiblePaths = {
            "tests/",           // Relative to build directory
            "../tests/",        // One level up from build
            "../../tests/",     // Two levels up from build/Debug
            "./tests/"          // Current directory
        };
        
        bool foundData = false;
        for (const auto& path : possiblePaths) {
            areaTestCases = readAreaTestCases(path + "area.csv");
            
            if (!areaTestCases.empty()) {
                testDataPath = path;
                foundData = true;
                break;
            }
        }
        
        // Verify we loaded data successfully
        ASSERT_TRUE(foundData) << "Failed to load test data from any of the expected locations";
        ASSERT_FALSE(areaTestCases.empty()) << "Failed to load area test cases from area.csv";
        
        std::cout << "Loaded " << areaTestCases.size() << " area test cases from " << testDataPath << std::endl;
    }
    
    std::vector<AreaTestCase> readAreaTestCases(const std::string& filename) {
        std::vector<AreaTestCase> testCases;
        std::ifstream file(filename);
        std::string line;
        
        // Skip header line
        if (std::getline(file, line)) {
            while (std::getline(file, line)) {
                if (line.empty()) continue;
                
                AreaTestCase testCase;
                std::stringstream ss(line);
                std::string cell;
                
                // Parse CSV: h_tx__meter,h_rx__meter,tx_siting_criteria,rx_siting_criteria,d__km,delta_h__meter,f__mhz,pol,epsilon,sigma,p
                if (std::getline(ss, cell, ',')) testCase.h_tx__meter = std::stod(cell);
                if (std::getline(ss, cell, ',')) testCase.h_rx__meter = std::stod(cell);
                if (std::getline(ss, cell, ',')) testCase.tx_siting_criteria = std::stoi(cell);
                if (std::getline(ss, cell, ',')) testCase.rx_siting_criteria = std::stoi(cell);
                if (std::getline(ss, cell, ',')) testCase.d__km = std::stod(cell);
                if (std::getline(ss, cell, ',')) testCase.delta_h__meter = std::stod(cell);
                if (std::getline(ss, cell, ',')) testCase.f__mhz = std::stod(cell);
                if (std::getline(ss, cell, ',')) testCase.pol = std::stoi(cell);
                if (std::getline(ss, cell, ',')) testCase.epsilon = std::stod(cell);
                if (std::getline(ss, cell, ',')) testCase.sigma = std::stod(cell);
                if (std::getline(ss, cell, ',')) testCase.p = std::stod(cell);
                
                testCases.push_back(testCase);
            }
        }
        file.close();
        return testCases;
    }
    
    std::string testDataPath;
    std::vector<AreaTestCase> areaTestCases;
};

// Test basic functionality with first test case
TEST_F(AreaExTest, BasicFunctionality_FirstTestCase) {
    ASSERT_FALSE(areaTestCases.empty());
    
    const AreaTestCase& testCase = areaTestCases[0];
    
    double A_db = 0.0;
    long warnings = 0;
    IntermediateValues interValues;
    
    int result = Area_Ex(
        testCase.h_tx__meter,
        testCase.h_rx__meter,
        testCase.tx_siting_criteria,
        testCase.rx_siting_criteria,
        testCase.d__km,
        testCase.delta_h__meter,
        testCase.f__mhz,
        testCase.pol,
        testCase.epsilon,
        testCase.sigma,
        testCase.p,
        &A_db,
        &warnings,
        &interValues
    );
    
    // Basic validation
    EXPECT_TRUE(result == SUCCESS || result == SUCCESS_WITH_WARNINGS);
    EXPECT_GT(A_db, 0.0) << "Transmission loss should be positive";
    EXPECT_LT(A_db, 500.0) << "Transmission loss should be reasonable (< 500 dB)";
    
    // Check intermediate values are populated
    EXPECT_DOUBLE_EQ(interValues.d__km, testCase.d__km) << "Distance should match input";
    EXPECT_GT(interValues.A_fs__db, 0.0) << "Free space loss should be positive";
    EXPECT_GE(interValues.A_ref__db, 0.0) << "Reference attenuation should be non-negative";
    EXPECT_DOUBLE_EQ(interValues.delta_h__meter, testCase.delta_h__meter) << "Delta H should match input";
    
    // Validate horizon data
    EXPECT_GE(interValues.d_hzn__meter[0], 0.0) << "TX horizon distance should be non-negative";
    EXPECT_GE(interValues.d_hzn__meter[1], 0.0) << "RX horizon distance should be non-negative";
    EXPECT_GE(interValues.h_e__meter[0], 0.0) << "TX effective height should be non-negative";
    EXPECT_GE(interValues.h_e__meter[1], 0.0) << "RX effective height should be non-negative";
}

// Test all provided test cases
TEST_F(AreaExTest, AllTestCases_ValidResults) {
    ASSERT_GT(areaTestCases.size(), 0) << "No valid test cases found";
    
    for (size_t i = 0; i < areaTestCases.size(); ++i) {
        const AreaTestCase& testCase = areaTestCases[i];
        
        double A_db = 0.0;
        long warnings = 0;
        IntermediateValues interValues;
        
        int result = Area_Ex(
            testCase.h_tx__meter,
            testCase.h_rx__meter,
            testCase.tx_siting_criteria,
            testCase.rx_siting_criteria,
            testCase.d__km,
            testCase.delta_h__meter,
            testCase.f__mhz,
            testCase.pol,
            testCase.epsilon,
            testCase.sigma,
            testCase.p,
            &A_db,
            &warnings,
            &interValues
        );
        
        // Test should succeed or succeed with warnings
        EXPECT_TRUE(result == SUCCESS || result == SUCCESS_WITH_WARNINGS) 
            << "Area test case " << i << " failed with error code: " << result;
        
        // Basic sanity checks on results
        EXPECT_GT(A_db, 0.0) << "Area test case " << i << ": Transmission loss should be positive";
        EXPECT_LT(A_db, 1000.0) << "Area test case " << i << ": Transmission loss should be reasonable";
        EXPECT_DOUBLE_EQ(interValues.d__km, testCase.d__km) << "Area test case " << i << ": Distance should match input";
        
        // Log results for debugging
        std::cout << "Area test case " << i << ": "
                  << "A_db=" << A_db 
                  << ", d_km=" << interValues.d__km
                  << ", warnings=0x" << std::hex << warnings << std::dec
                  << ", mode=" << interValues.mode << std::endl;
    }
}

// Test error conditions with invalid inputs
TEST_F(AreaExTest, InvalidInputs_ErrorHandling) {
    double A_db = 0.0;
    long warnings = 0;
    IntermediateValues interValues;
    
    // Test with invalid frequency (too low)
    int result = Area_Ex(
        10.0,   // h_tx
        2.0,    // h_rx
        0,      // tx_siting_criteria
        0,      // rx_siting_criteria
        10.0,   // d__km
        5.0,    // delta_h__meter
        15.0,   // f_mhz - too low (should be >= 20)
        POLARIZATION__HORIZONTAL,
        15.0,   // epsilon
        0.008,  // sigma
        50.0,   // p
        &A_db,
        &warnings,
        &interValues
    );
    
    EXPECT_EQ(result, ERROR__FREQUENCY);
    
    // Test with invalid polarization
    result = Area_Ex(
        10.0,   // h_tx
        2.0,    // h_rx
        0,      // tx_siting_criteria
        0,      // rx_siting_criteria
        10.0,   // d__km
        5.0,    // delta_h__meter
        1000.0, // f_mhz
        99,     // pol - invalid
        15.0,   // epsilon
        0.008,  // sigma
        50.0,   // p
        &A_db,
        &warnings,
        &interValues
    );
    
    EXPECT_EQ(result, ERROR__POLARIZATION);
    
    // Test with invalid percentage
    result = Area_Ex(
        10.0,   // h_tx
        2.0,    // h_rx
        0,      // tx_siting_criteria
        0,      // rx_siting_criteria
        10.0,   // d__km
        5.0,    // delta_h__meter
        1000.0, // f_mhz
        POLARIZATION__HORIZONTAL,
        15.0,   // epsilon
        0.008,  // sigma
        150.0,  // p - too high (should be < 100)
        &A_db,
        &warnings,
        &interValues
    );
    
    EXPECT_EQ(result, ERROR__INVALID_PERCENTAGE);
}

// Test warning conditions
TEST_F(AreaExTest, WarningConditions_SuccessWithWarnings) {
    double A_db = 0.0;
    long warnings = 0;
    IntermediateValues interValues;
    
    // Test with parameters that should generate warnings
    int result = Area_Ex(
        0.8,    // h_tx - below warning threshold (< 1.0)
        1200.0, // h_rx - above warning threshold (> 1000.0)
        0,      // tx_siting_criteria
        0,      // rx_siting_criteria
        10.0,   // d__km
        5.0,    // delta_h__meter
        35.0,   // f_mhz - below warning threshold (< 40.0)
        POLARIZATION__VERTICAL,
        15.0,   // epsilon
        0.008,  // sigma
        50.0,   // p
        &A_db,
        &warnings,
        &interValues
    );
    
    EXPECT_EQ(result, SUCCESS_WITH_WARNINGS);
    EXPECT_NE(warnings & WARN__TX_TERMINAL_HEIGHT, 0);
    EXPECT_NE(warnings & WARN__RX_TERMINAL_HEIGHT, 0);
    EXPECT_NE(warnings & WARN__FREQUENCY, 0);
    EXPECT_GT(A_db, 0.0);
}

// Test different polarizations
TEST_F(AreaExTest, DifferentPolarizations_ValidResults) {
    ASSERT_FALSE(areaTestCases.empty());
    
    const AreaTestCase& baseCase = areaTestCases[0];
    
    // Test horizontal polarization
    double A_db_horizontal = 0.0;
    long warnings_h = 0;
    IntermediateValues interValues_h;
    
    int result_h = Area_Ex(
        baseCase.h_tx__meter,
        baseCase.h_rx__meter,
        baseCase.tx_siting_criteria,
        baseCase.rx_siting_criteria,
        baseCase.d__km,
        baseCase.delta_h__meter,
        baseCase.f__mhz,
        POLARIZATION__HORIZONTAL,
        baseCase.epsilon,
        baseCase.sigma,
        baseCase.p,
        &A_db_horizontal,
        &warnings_h,
        &interValues_h
    );
    
    // Test vertical polarization
    double A_db_vertical = 0.0;
    long warnings_v = 0;
    IntermediateValues interValues_v;
    
    int result_v = Area_Ex(
        baseCase.h_tx__meter,
        baseCase.h_rx__meter,
        baseCase.tx_siting_criteria,
        baseCase.rx_siting_criteria,
        baseCase.d__km,
        baseCase.delta_h__meter,
        baseCase.f__mhz,
        POLARIZATION__VERTICAL,
        baseCase.epsilon,
        baseCase.sigma,
        baseCase.p,
        &A_db_vertical,
        &warnings_v,
        &interValues_v
    );
    
    // Both should succeed
    EXPECT_TRUE(result_h == SUCCESS || result_h == SUCCESS_WITH_WARNINGS);
    EXPECT_TRUE(result_v == SUCCESS || result_v == SUCCESS_WITH_WARNINGS);
    
    // Both should produce valid results
    EXPECT_GT(A_db_horizontal, 0.0);
    EXPECT_GT(A_db_vertical, 0.0);
    
    // Results might differ due to polarization effects
    std::cout << "Area Horizontal polarization: A_db = " << A_db_horizontal 
              << ", Area Vertical polarization: A_db = " << A_db_vertical << std::endl;
}

// Test different siting criteria
TEST_F(AreaExTest, DifferentSitingCriteria_ValidResults) {
    ASSERT_FALSE(areaTestCases.empty());
    
    const AreaTestCase& baseCase = areaTestCases[0];
    
    // Test different combinations of siting criteria (0, 1 only - 2 is invalid)
    std::vector<std::pair<int, int>> sitingCombinations = {
        {0, 0}, {0, 1},
        {1, 0}, {1, 1}
    };
    
    for (const auto& combo : sitingCombinations) {
        double A_db = 0.0;
        long warnings = 0;
        IntermediateValues interValues;
        
        int result = Area_Ex(
            baseCase.h_tx__meter,
            baseCase.h_rx__meter,
            combo.first,    // tx_siting_criteria
            combo.second,   // rx_siting_criteria
            baseCase.d__km,
            baseCase.delta_h__meter,
            baseCase.f__mhz,
            baseCase.pol,
            baseCase.epsilon,
            baseCase.sigma,
            baseCase.p,
            &A_db,
            &warnings,
            &interValues
        );
        
        EXPECT_TRUE(result == SUCCESS || result == SUCCESS_WITH_WARNINGS)
            << "Siting criteria (" << combo.first << "," << combo.second << ") failed";
        EXPECT_GT(A_db, 0.0) << "Siting criteria (" << combo.first << "," << combo.second << ") produced invalid loss";
        
        std::cout << "Siting criteria (" << combo.first << "," << combo.second 
                  << "): A_db = " << A_db << " dB" << std::endl;
    }
}

// Test intermediate values consistency
TEST_F(AreaExTest, IntermediateValues_Consistency) {
    ASSERT_FALSE(areaTestCases.empty());
    
    const AreaTestCase& testCase = areaTestCases[0];
    
    double A_db = 0.0;
    long warnings = 0;
    IntermediateValues interValues;
    
    int result = Area_Ex(
        testCase.h_tx__meter,
        testCase.h_rx__meter,
        testCase.tx_siting_criteria,
        testCase.rx_siting_criteria,
        testCase.d__km,
        testCase.delta_h__meter,
        testCase.f__mhz,
        testCase.pol,
        testCase.epsilon,
        testCase.sigma,
        testCase.p,
        &A_db,
        &warnings,
        &interValues
    );
    
    EXPECT_TRUE(result == SUCCESS || result == SUCCESS_WITH_WARNINGS);
    
    // Verify intermediate values consistency
    EXPECT_DOUBLE_EQ(interValues.d__km, testCase.d__km) << "Distance should match input";
    EXPECT_DOUBLE_EQ(interValues.delta_h__meter, testCase.delta_h__meter) << "Delta H should match input";
    EXPECT_GT(interValues.A_fs__db, 0.0) << "Free space loss should be positive";
    
    // Free space loss should be less than total loss (in most cases)
    EXPECT_LE(interValues.A_fs__db, A_db + 50.0) << "Free space loss should be reasonable compared to total loss";
    
    // Propagation mode should be valid
    EXPECT_GE(interValues.mode, MODE__NOT_SET);
    EXPECT_NE(interValues.mode, MODE__NOT_SET) << "Propagation mode should be determined";
    
    // Horizon angles should be reasonable (in radians, typically small)
    EXPECT_GE(interValues.theta_hzn[0], -0.5) << "TX horizon angle should be reasonable";
    EXPECT_LE(interValues.theta_hzn[0], 0.5) << "TX horizon angle should be reasonable";
    EXPECT_GE(interValues.theta_hzn[1], -0.5) << "RX horizon angle should be reasonable";
    EXPECT_LE(interValues.theta_hzn[1], 0.5) << "RX horizon angle should be reasonable";
}

// Test intermediate values consistency
TEST_F(PointToPointExTest, IntermediateValues_Consistency) {
    ASSERT_FALSE(testCases.empty());
    ASSERT_FALSE(pflDataList.empty());
    ASSERT_TRUE(pflDataList[0].isValid);
    
    const P2PTestCase& testCase = testCases[0];
    const PFLData& pflData = pflDataList[0];
    
    double A_db = 0.0;
    long warnings = 0;
    IntermediateValues interValues;
    
    int result = PointToPoint_Ex(
        testCase.h_tx__meter,
        testCase.h_rx__meter,
        const_cast<double*>(pflData.pfl.data()),
        testCase.f__mhz,
        testCase.pol,
        testCase.epsilon,
        testCase.sigma,
        testCase.p,
        &A_db,
        &warnings,
        &interValues
    );
    
    EXPECT_TRUE(result == SUCCESS || result == SUCCESS_WITH_WARNINGS);
    
    // Verify intermediate values consistency
    EXPECT_GT(interValues.d__km, 0.0);
    EXPECT_GT(interValues.A_fs__db, 0.0);
    
    // Free space loss should be less than total loss (in most cases)
    EXPECT_LE(interValues.A_fs__db, A_db + 50.0) << "Free space loss should be reasonable compared to total loss";
    
    // Distance should match PFL data
    double expected_distance_km = pflData.pfl[0] * pflData.pfl[1] / 1000.0;
    EXPECT_DOUBLE_EQ(interValues.d__km, expected_distance_km) << "Distance should match PFL calculation";
    
    // Propagation mode should be valid
    EXPECT_GE(interValues.mode, MODE__NOT_SET);
    EXPECT_NE(interValues.mode, MODE__NOT_SET) << "Propagation mode should be determined";
    
    // Horizon angles should be reasonable (in radians, typically small)
    EXPECT_GE(interValues.theta_hzn[0], -0.5) << "TX horizon angle should be reasonable";
    EXPECT_LE(interValues.theta_hzn[0], 0.5) << "TX horizon angle should be reasonable";
    EXPECT_GE(interValues.theta_hzn[1], -0.5) << "RX horizon angle should be reasonable";
    EXPECT_LE(interValues.theta_hzn[1], 0.5) << "RX horizon angle should be reasonable";
}

// ===================== Approval Tests =====================

// Helper function to format test results for approval testing
std::string formatPointToPointResults(const P2PTestCase& testCase, 
                                    const PFLData& pflData, 
                                    size_t testIndex) {
    std::stringstream ss;
    ss << std::fixed << std::setprecision(6);
    
    ss << "Test Case " << testIndex << ":\n";
    ss << "  Input Parameters:\n";
    ss << "    h_tx (m):     " << testCase.h_tx__meter << "\n";
    ss << "    h_rx (m):     " << testCase.h_rx__meter << "\n";
    ss << "    f (MHz):      " << testCase.f__mhz << "\n";
    ss << "    pol:          " << testCase.pol << " (" 
       << (testCase.pol == POLARIZATION__HORIZONTAL ? "Horizontal" : "Vertical") << ")\n";
    ss << "    epsilon:      " << testCase.epsilon << "\n";
    ss << "    sigma:        " << testCase.sigma << "\n";
    ss << "    p (%):        " << testCase.p << "\n";
    
    // Skip invalid PFL data
    if (!pflData.isValid) {
        ss << "  Result:         SKIPPED (Invalid PFL data)\n";
        return ss.str();
    }
    
    ss << "  PFL Data:\n";
    ss << "    Points:       " << (int)pflData.pfl[0] << "\n";
    ss << "    Increment:    " << pflData.pfl[1] << " m\n";
    ss << "    Total Dist:   " << (pflData.pfl[0] * pflData.pfl[1] / 1000.0) << " km\n";
    
    double A_db = 0.0;
    long warnings = 0;
    IntermediateValues interValues;
    
    int result = PointToPoint_Ex(
        testCase.h_tx__meter,
        testCase.h_rx__meter,
        const_cast<double*>(pflData.pfl.data()),
        testCase.f__mhz,
        testCase.pol,
        testCase.epsilon,
        testCase.sigma,
        testCase.p,
        &A_db,
        &warnings,
        &interValues
    );
    
    ss << "  Results:\n";
    ss << "    Return Code:  " << result;
    
    if (result == SUCCESS) {
        ss << " (SUCCESS)\n";
    } else if (result == SUCCESS_WITH_WARNINGS) {
        ss << " (SUCCESS_WITH_WARNINGS)\n";
    } else {
        ss << " (ERROR)\n";
    }
    
    if (result == SUCCESS || result == SUCCESS_WITH_WARNINGS) {
        ss << "    A_db:         " << A_db << " dB\n";
        ss << "    Warnings:     0x" << std::hex << warnings << std::dec << "\n";
        
        ss << "  Intermediate Values:\n";
        ss << "    d_km:         " << interValues.d__km << " km\n";
        ss << "    A_fs_db:      " << interValues.A_fs__db << " dB\n";
        ss << "    A_ref_db:     " << interValues.A_ref__db << " dB\n";
        ss << "    delta_h:      " << interValues.delta_h__meter << " m\n";
        ss << "    mode:         " << interValues.mode << "\n";
        
        ss << "  Horizon Data:\n";
        ss << "    TX theta:     " << interValues.theta_hzn[0] << " rad\n";
        ss << "    RX theta:     " << interValues.theta_hzn[1] << " rad\n";
        ss << "    TX d_hzn:     " << interValues.d_hzn__meter[0] << " m\n";
        ss << "    RX d_hzn:     " << interValues.d_hzn__meter[1] << " m\n";
        ss << "    TX h_e:       " << interValues.h_e__meter[0] << " m\n";
        ss << "    RX h_e:       " << interValues.h_e__meter[1] << " m\n";
        
        // Decode warnings if present
        if (warnings != 0) {
            ss << "  Warning Details:\n";
            if (warnings & WARN__TX_TERMINAL_HEIGHT) 
                ss << "    - TX height near limits\n";
            if (warnings & WARN__RX_TERMINAL_HEIGHT) 
                ss << "    - RX height near limits\n";
            if (warnings & WARN__FREQUENCY) 
                ss << "    - Frequency near limits\n";
            if (warnings & WARN__PATH_DISTANCE_TOO_BIG_1) 
                ss << "    - Path distance near upper limit\n";
            if (warnings & WARN__PATH_DISTANCE_TOO_BIG_2) 
                ss << "    - Path distance large\n";
            if (warnings & WARN__PATH_DISTANCE_TOO_SMALL_1) 
                ss << "    - Path distance near lower limit\n";
            if (warnings & WARN__PATH_DISTANCE_TOO_SMALL_2) 
                ss << "    - Path distance small\n";
            if (warnings & WARN__TX_HORIZON_ANGLE) 
                ss << "    - TX horizon angle large\n";
            if (warnings & WARN__RX_HORIZON_ANGLE) 
                ss << "    - RX horizon angle large\n";
            if (warnings & WARN__TX_HORIZON_DISTANCE_1) 
                ss << "    - TX horizon distance < 1/10 smooth earth\n";
            if (warnings & WARN__RX_HORIZON_DISTANCE_1) 
                ss << "    - RX horizon distance < 1/10 smooth earth\n";
            if (warnings & WARN__TX_HORIZON_DISTANCE_2) 
                ss << "    - TX horizon distance > 3x smooth earth\n";
            if (warnings & WARN__RX_HORIZON_DISTANCE_2) 
                ss << "    - RX horizon distance > 3x smooth earth\n";
        }
    } else {
        // Decode error codes
        ss << "  Error Details:\n";
        switch (result) {
            case ERROR__TX_TERMINAL_HEIGHT:
                ss << "    - TX terminal height out of range\n";
                break;
            case ERROR__RX_TERMINAL_HEIGHT:
                ss << "    - RX terminal height out of range\n";
                break;
            case ERROR__FREQUENCY:
                ss << "    - Frequency out of range\n";
                break;
            case ERROR__POLARIZATION:
                ss << "    - Invalid polarization\n";
                break;
            case ERROR__EPSILON:
                ss << "    - Epsilon out of range\n";
                break;
            case ERROR__SIGMA:
                ss << "    - Sigma out of range\n";
                break;
            case ERROR__INVALID_PERCENTAGE:
                ss << "    - Invalid percentage\n";
                break;
            default:
                ss << "    - Unknown error code: " << result << "\n";
                break;
        }
    }
    
    return ss.str();
}

// Approval test using the official ApprovalTests framework
TEST_F(PointToPointExTest, ApprovalTest_AllResults) {
    // Ensure we have matching test cases and PFL data
    size_t maxTests = std::min(testCases.size(), pflDataList.size());
    ASSERT_GT(maxTests, 0) << "No valid test cases found for approval test";
    
    std::stringstream allResults;
    allResults << "ILM PointToPoint_Ex Approval Test Results\n";
    allResults << "Generated on: October 22, 2025\n";
    allResults << "Test Cases: " << maxTests << "\n";
    allResults << "========================================\n\n";
    
    int successCount = 0;
    int warningCount = 0;
    int errorCount = 0;
    
    for (size_t i = 0; i < maxTests; ++i) {
        const P2PTestCase& testCase = testCases[i];
        const PFLData& pflData = pflDataList[i];
        
        allResults << formatPointToPointResults(testCase, pflData, i);
        allResults << "\n";
        
        // Count results for summary
        if (pflData.isValid) {
            double A_db = 0.0;
            long warnings = 0;
            IntermediateValues interValues;
            
            int result = PointToPoint_Ex(
                testCase.h_tx__meter,
                testCase.h_rx__meter,
                const_cast<double*>(pflData.pfl.data()),
                testCase.f__mhz,
                testCase.pol,
                testCase.epsilon,
                testCase.sigma,
                testCase.p,
                &A_db,
                &warnings,
                &interValues
            );
            
            if (result == SUCCESS) {
                successCount++;
            } else if (result == SUCCESS_WITH_WARNINGS) {
                warningCount++;
            } else {
                errorCount++;
            }
        }
    }
    
    // Add summary
    allResults << "========================================\n";
    allResults << "Summary:\n";
    allResults << "  Total Tests:  " << maxTests << "\n";
    allResults << "  Success:      " << successCount << "\n";
    allResults << "  Warnings:     " << warningCount << "\n";
    allResults << "  Errors:       " << errorCount << "\n";
    allResults << "  Success Rate: " << std::fixed << std::setprecision(1) 
               << (100.0 * (successCount + warningCount) / maxTests) << "%\n";
    
    // Use ApprovalTests to verify the results
    ApprovalTests::Approvals::verify(allResults.str());
}

// Individual approval tests for each test case
TEST_F(PointToPointExTest, ApprovalTest_FirstCase) {
    ASSERT_FALSE(testCases.empty());
    ASSERT_FALSE(pflDataList.empty());
    ASSERT_TRUE(pflDataList[0].isValid);
    
    const P2PTestCase& testCase = testCases[0];
    const PFLData& pflData = pflDataList[0];
    
    std::string result = formatPointToPointResults(testCase, pflData, 0);
    ApprovalTests::Approvals::verify(result);
}

TEST_F(PointToPointExTest, ApprovalTest_SecondCase) {
    ASSERT_GT(testCases.size(), 1);
    ASSERT_GT(pflDataList.size(), 1);
    ASSERT_TRUE(pflDataList[1].isValid);
    
    const P2PTestCase& testCase = testCases[1];
    const PFLData& pflData = pflDataList[1];
    
    std::string result = formatPointToPointResults(testCase, pflData, 1);
    ApprovalTests::Approvals::verify(result);
}

TEST_F(PointToPointExTest, ApprovalTest_ThirdCase) {
    ASSERT_GT(testCases.size(), 2);
    ASSERT_GT(pflDataList.size(), 2);
    ASSERT_TRUE(pflDataList[2].isValid);
    
    const P2PTestCase& testCase = testCases[2];
    const PFLData& pflData = pflDataList[2];
    
    std::string result = formatPointToPointResults(testCase, pflData, 2);
    ApprovalTests::Approvals::verify(result);
}

TEST_F(PointToPointExTest, ApprovalTest_FourthCase) {
    ASSERT_GT(testCases.size(), 3);
    ASSERT_GT(pflDataList.size(), 3);
    ASSERT_TRUE(pflDataList[3].isValid);
    
    const P2PTestCase& testCase = testCases[3];
    const PFLData& pflData = pflDataList[3];
    
    std::string result = formatPointToPointResults(testCase, pflData, 3);
    ApprovalTests::Approvals::verify(result);
}

// Simplified approval test for quick validation
TEST_F(PointToPointExTest, QuickApprovalTest_Summary) {
    ASSERT_FALSE(testCases.empty());
    ASSERT_FALSE(pflDataList.empty());
    ASSERT_TRUE(pflDataList[0].isValid);
    
    const P2PTestCase& testCase = testCases[0];
    const PFLData& pflData = pflDataList[0];
    
    double A_db = 0.0;
    long warnings = 0;
    IntermediateValues interValues;
    
    int result = PointToPoint_Ex(
        testCase.h_tx__meter,
        testCase.h_rx__meter,
        const_cast<double*>(pflData.pfl.data()),
        testCase.f__mhz,
        testCase.pol,
        testCase.epsilon,
        testCase.sigma,
        testCase.p,
        &A_db,
        &warnings,
        &interValues
    );
    
    // Create a simple approval result for the first test case
    std::stringstream quickResult;
    quickResult << std::fixed << std::setprecision(3);
    quickResult << "Quick Approval Test - First Case Summary\n";
    quickResult << "Input: h_tx=" << testCase.h_tx__meter 
                << "m, h_rx=" << testCase.h_rx__meter 
                << "m, f=" << testCase.f__mhz << "MHz\n";
    quickResult << "Result: A_db=" << A_db 
                << "dB, warnings=0x" << std::hex << warnings << std::dec
                << ", mode=" << interValues.mode << "\n";
    quickResult << "Distance: " << interValues.d__km << "km\n";
    quickResult << "Status: " << (result == SUCCESS ? "SUCCESS" : 
                                  result == SUCCESS_WITH_WARNINGS ? "SUCCESS_WITH_WARNINGS" : "ERROR");
    
    ApprovalTests::Approvals::verify(quickResult.str());
    
    // Basic validation for approval testing
    EXPECT_TRUE(result == SUCCESS || result == SUCCESS_WITH_WARNINGS);
    EXPECT_GT(A_db, 0.0);
    EXPECT_LT(A_db, 1000.0);
}

// ===================== Area_Ex Approval Tests =====================

// Helper function to format area test results for approval testing
std::string formatAreaResults(const AreaTestCase& testCase, size_t testIndex) {
    std::stringstream ss;
    ss << std::fixed << std::setprecision(6);
    
    ss << "Area Test Case " << testIndex << ":\n";
    ss << "  Input Parameters:\n";
    ss << "    h_tx (m):        " << testCase.h_tx__meter << "\n";
    ss << "    h_rx (m):        " << testCase.h_rx__meter << "\n";
    ss << "    tx_siting:       " << testCase.tx_siting_criteria << "\n";
    ss << "    rx_siting:       " << testCase.rx_siting_criteria << "\n";
    ss << "    d (km):          " << testCase.d__km << "\n";
    ss << "    delta_h (m):     " << testCase.delta_h__meter << "\n";
    ss << "    f (MHz):         " << testCase.f__mhz << "\n";
    ss << "    pol:             " << testCase.pol << " (" 
       << (testCase.pol == POLARIZATION__HORIZONTAL ? "Horizontal" : "Vertical") << ")\n";
    ss << "    epsilon:         " << testCase.epsilon << "\n";
    ss << "    sigma:           " << testCase.sigma << "\n";
    ss << "    p (%):           " << testCase.p << "\n";
    
    double A_db = 0.0;
    long warnings = 0;
    IntermediateValues interValues;
    
    int result = Area_Ex(
        testCase.h_tx__meter,
        testCase.h_rx__meter,
        testCase.tx_siting_criteria,
        testCase.rx_siting_criteria,
        testCase.d__km,
        testCase.delta_h__meter,
        testCase.f__mhz,
        testCase.pol,
        testCase.epsilon,
        testCase.sigma,
        testCase.p,
        &A_db,
        &warnings,
        &interValues
    );
    
    ss << "  Results:\n";
    ss << "    Return Code:     " << result;
    
    if (result == SUCCESS) {
        ss << " (SUCCESS)\n";
    } else if (result == SUCCESS_WITH_WARNINGS) {
        ss << " (SUCCESS_WITH_WARNINGS)\n";
    } else {
        ss << " (ERROR)\n";
    }
    
    if (result == SUCCESS || result == SUCCESS_WITH_WARNINGS) {
        ss << "    A_db:            " << A_db << " dB\n";
        ss << "    Warnings:        0x" << std::hex << warnings << std::dec << "\n";
        
        ss << "  Intermediate Values:\n";
        ss << "    d_km:            " << interValues.d__km << " km\n";
        ss << "    A_fs_db:         " << interValues.A_fs__db << " dB\n";
        ss << "    A_ref_db:        " << interValues.A_ref__db << " dB\n";
        ss << "    delta_h:         " << interValues.delta_h__meter << " m\n";
        ss << "    mode:            " << interValues.mode << "\n";
        
        ss << "  Horizon Data:\n";
        ss << "    TX theta:        " << interValues.theta_hzn[0] << " rad\n";
        ss << "    RX theta:        " << interValues.theta_hzn[1] << " rad\n";
        ss << "    TX d_hzn:        " << interValues.d_hzn__meter[0] << " m\n";
        ss << "    RX d_hzn:        " << interValues.d_hzn__meter[1] << " m\n";
        ss << "    TX h_e:          " << interValues.h_e__meter[0] << " m\n";
        ss << "    RX h_e:          " << interValues.h_e__meter[1] << " m\n";
        
        // Decode warnings if present
        if (warnings != 0) {
            ss << "  Warning Details:\n";
            if (warnings & WARN__TX_TERMINAL_HEIGHT) 
                ss << "    - TX height near limits\n";
            if (warnings & WARN__RX_TERMINAL_HEIGHT) 
                ss << "    - RX height near limits\n";
            if (warnings & WARN__FREQUENCY) 
                ss << "    - Frequency near limits\n";
            if (warnings & WARN__PATH_DISTANCE_TOO_BIG_1) 
                ss << "    - Path distance near upper limit\n";
            if (warnings & WARN__PATH_DISTANCE_TOO_BIG_2) 
                ss << "    - Path distance large\n";
            if (warnings & WARN__PATH_DISTANCE_TOO_SMALL_1) 
                ss << "    - Path distance near lower limit\n";
            if (warnings & WARN__PATH_DISTANCE_TOO_SMALL_2) 
                ss << "    - Path distance small\n";
            if (warnings & WARN__TX_HORIZON_ANGLE) 
                ss << "    - TX horizon angle large\n";
            if (warnings & WARN__RX_HORIZON_ANGLE) 
                ss << "    - RX horizon angle large\n";
            if (warnings & WARN__TX_HORIZON_DISTANCE_1) 
                ss << "    - TX horizon distance < 1/10 smooth earth\n";
            if (warnings & WARN__RX_HORIZON_DISTANCE_1) 
                ss << "    - RX horizon distance < 1/10 smooth earth\n";
            if (warnings & WARN__TX_HORIZON_DISTANCE_2) 
                ss << "    - TX horizon distance > 3x smooth earth\n";
            if (warnings & WARN__RX_HORIZON_DISTANCE_2) 
                ss << "    - RX horizon distance > 3x smooth earth\n";
        }
    } else {
        // Decode error codes
        ss << "  Error Details:\n";
        switch (result) {
            case ERROR__TX_TERMINAL_HEIGHT:
                ss << "    - TX terminal height out of range\n";
                break;
            case ERROR__RX_TERMINAL_HEIGHT:
                ss << "    - RX terminal height out of range\n";
                break;
            case ERROR__FREQUENCY:
                ss << "    - Frequency out of range\n";
                break;
            case ERROR__POLARIZATION:
                ss << "    - Invalid polarization\n";
                break;
            case ERROR__EPSILON:
                ss << "    - Epsilon out of range\n";
                break;
            case ERROR__SIGMA:
                ss << "    - Sigma out of range\n";
                break;
            case ERROR__INVALID_PERCENTAGE:
                ss << "    - Invalid percentage\n";
                break;
            default:
                ss << "    - Unknown error code: " << result << "\n";
                break;
        }
    }
    
    return ss.str();
}

// Approval test using the official ApprovalTests framework
TEST_F(AreaExTest, ApprovalTest_AllResults) {
    ASSERT_GT(areaTestCases.size(), 0) << "No valid area test cases found for approval test";
    
    std::stringstream allResults;
    allResults << "ILM Area_Ex Approval Test Results\n";
    allResults << "Generated on: October 24, 2025\n";
    allResults << "Test Cases: " << areaTestCases.size() << "\n";
    allResults << "========================================\n\n";
    
    int successCount = 0;
    int warningCount = 0;
    int errorCount = 0;
    
    for (size_t i = 0; i < areaTestCases.size(); ++i) {
        const AreaTestCase& testCase = areaTestCases[i];
        
        allResults << formatAreaResults(testCase, i);
        allResults << "\n";
        
        // Count results for summary
        double A_db = 0.0;
        long warnings = 0;
        IntermediateValues interValues;
        
        int result = Area_Ex(
            testCase.h_tx__meter,
            testCase.h_rx__meter,
            testCase.tx_siting_criteria,
            testCase.rx_siting_criteria,
            testCase.d__km,
            testCase.delta_h__meter,
            testCase.f__mhz,
            testCase.pol,
            testCase.epsilon,
            testCase.sigma,
            testCase.p,
            &A_db,
            &warnings,
            &interValues
        );
        
        if (result == SUCCESS) {
            successCount++;
        } else if (result == SUCCESS_WITH_WARNINGS) {
            warningCount++;
        } else {
            errorCount++;
        }
    }
    
    // Add summary
    allResults << "========================================\n";
    allResults << "Summary:\n";
    allResults << "  Total Tests:     " << areaTestCases.size() << "\n";
    allResults << "  Success:         " << successCount << "\n";
    allResults << "  Warnings:        " << warningCount << "\n";
    allResults << "  Errors:          " << errorCount << "\n";
    allResults << "  Success Rate:    " << std::fixed << std::setprecision(1) 
               << (100.0 * (successCount + warningCount) / areaTestCases.size()) << "%\n";
    
    // Use ApprovalTests to verify the results
    ApprovalTests::Approvals::verify(allResults.str());
}

// Individual approval tests for each area test case
TEST_F(AreaExTest, ApprovalTest_FirstCase) {
    ASSERT_FALSE(areaTestCases.empty());
    
    const AreaTestCase& testCase = areaTestCases[0];
    
    std::string result = formatAreaResults(testCase, 0);
    ApprovalTests::Approvals::verify(result);
}

TEST_F(AreaExTest, ApprovalTest_SecondCase) {
    ASSERT_GT(areaTestCases.size(), 1);
    
    const AreaTestCase& testCase = areaTestCases[1];
    
    std::string result = formatAreaResults(testCase, 1);
    ApprovalTests::Approvals::verify(result);
}

TEST_F(AreaExTest, ApprovalTest_ThirdCase) {
    ASSERT_GT(areaTestCases.size(), 2);
    
    const AreaTestCase& testCase = areaTestCases[2];
    
    std::string result = formatAreaResults(testCase, 2);
    ApprovalTests::Approvals::verify(result);
}

TEST_F(AreaExTest, ApprovalTest_FourthCase) {
    ASSERT_GT(areaTestCases.size(), 3);
    
    const AreaTestCase& testCase = areaTestCases[3];
    
    std::string result = formatAreaResults(testCase, 3);
    ApprovalTests::Approvals::verify(result);
}

TEST_F(AreaExTest, ApprovalTest_FifthCase) {
    ASSERT_GT(areaTestCases.size(), 4);
    
    const AreaTestCase& testCase = areaTestCases[4];
    
    std::string result = formatAreaResults(testCase, 4);
    ApprovalTests::Approvals::verify(result);
}

// Simplified approval test for quick validation
TEST_F(AreaExTest, QuickApprovalTest_Summary) {
    ASSERT_FALSE(areaTestCases.empty());
    
    const AreaTestCase& testCase = areaTestCases[0];
    
    double A_db = 0.0;
    long warnings = 0;
    IntermediateValues interValues;
    
    int result = Area_Ex(
        testCase.h_tx__meter,
        testCase.h_rx__meter,
        testCase.tx_siting_criteria,
        testCase.rx_siting_criteria,
        testCase.d__km,
        testCase.delta_h__meter,
        testCase.f__mhz,
        testCase.pol,
        testCase.epsilon,
        testCase.sigma,
        testCase.p,
        &A_db,
        &warnings,
        &interValues
    );
    
    // Create a simple approval result for the first area test case
    std::stringstream quickResult;
    quickResult << std::fixed << std::setprecision(3);
    quickResult << "Quick Area Approval Test - First Case Summary\n";
    quickResult << "Input: h_tx=" << testCase.h_tx__meter 
                << "m, h_rx=" << testCase.h_rx__meter 
                << "m, f=" << testCase.f__mhz << "MHz\n";
    quickResult << "       d=" << testCase.d__km << "km, delta_h=" << testCase.delta_h__meter 
                << "m, siting=(" << testCase.tx_siting_criteria << "," << testCase.rx_siting_criteria << ")\n";
    quickResult << "Result: A_db=" << A_db 
                << "dB, warnings=0x" << std::hex << warnings << std::dec
                << ", mode=" << interValues.mode << "\n";
    quickResult << "Status: " << (result == SUCCESS ? "SUCCESS" : 
                                  result == SUCCESS_WITH_WARNINGS ? "SUCCESS_WITH_WARNINGS" : "ERROR");
    
    ApprovalTests::Approvals::verify(quickResult.str());
    
    // Basic validation for approval testing
    EXPECT_TRUE(result == SUCCESS || result == SUCCESS_WITH_WARNINGS);
    EXPECT_GT(A_db, 0.0);
    EXPECT_LT(A_db, 1000.0);
}





