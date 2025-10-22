#include <gtest/gtest.h>
#include "ilm.h"        // Include your shared library header
#include "Enums.h"      // For polarization constants
#include "Errors.h"     // For error constants
#include "Warnings.h"   // For warning constants

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




