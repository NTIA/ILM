# -*- coding: utf-8 -*-
"""Python wrapper for the Irregular Lunar Model (ILM).

This class provides a Python interface to ILM.dll using ctypes.

@author: Erik Hill, 2024
"""
# %% imports
# standard
import os
import ctypes as ct
import errno

# third party
import numpy as np

# local


# %%
class ILM:
    """Python wrapper class for the Irregular Lunar Model (ILM).

    This class provides an interface to the ILM.dll Library functions from
    Python through a ctypes interface.
    This class requires the C/C++ Dynamic Linked Library ILM.dll.

    The following functions are provided from ILM.dll.

        char const* compile_time();

        char const* version();

        int PointToPoint(
            double h_tx__meter,
            double h_rx__meter,
            double pfl[],
            double f__mhz,
            int pol,
            double epsilon,
            double sigma,
            double p,
            double* A__db,
            long* warnings
        );

        int Area(
            double h_tx__meter,
            double h_rx__meter,
            int tx_site_criteria,
            int rx_site_criteria,
            double d__km,
            double delta_h__meter,
            double f__mhz,
            int pol,
            double epsilon,
            double sigma,
            double p,
            double* A__db,
            long* warnings
        );

    Note
    ----
    Not all ILM.dll functions have been added to this wrapper.
    ILM.dll functions that do not have a basic ctypes interface cannot be added
    to this wrapper.
    """

    # %%
    def __init__(
            self,
            dll_path: str = None
    ):
        """Initialize ILM wrapper.

        Construct attributes for the ILM class by interfacing with the ILM.dll
        file.

        If no dll_path is given, the default path is searched.
        If the .dll is not found on the default path, an error is given.

        Parameters
        ----------
        dll_path : str, optional (default=None)
            The file path of the 64-bit ILM.dll to use.
            If dll_path is not provided, the default repository location is
            searched.
        """
        if dll_path is None:
            # Try the default ILM.dll repository location.
            dll_path = r"..\..\Visual_Studio\ILM\ILM_DLL\x64\Release\ILM.dll"

        # Ensure the .dll exists and can be read.
        try:
            with open(dll_path, 'r') as _:
                # The .dll exists and can be read.
                pass
        except FileNotFoundError:
            # The .dll can not be read.
            raise FileNotFoundError(
                errno.ENOENT,
                os.strerror(errno.ENOENT),
                dll_path
            )

        # Load the .dll.
        self.ilm_dll = ct.cdll.LoadLibrary(dll_path)

        # Get the .dll compile time.
        self.compile_time = getattr(self.ilm_dll, 'compile_time')
        self.compile_time.argtypes = None
        self.compile_time.restype = ct.c_char_p
        self.compile_time = self.ilm_dll.compile_time().decode('utf-8')

        # Get the .dll version.
        self.version = getattr(self.ilm_dll, 'version')
        self.version.argtypes = None
        self.version.restype = ct.c_char_p
        self.version = self.ilm_dll.version().decode('utf-8')

        # Get the PointToPoint method.
        self.PointToPoint = getattr(self.ilm_dll, 'PointToPoint')
        self.PointToPoint.argtypes = [
            ct.c_double,
            ct.c_double,
            np.ctypeslib.ndpointer(ct.c_double, flags='C_CONTIGUOUS'),
            ct.c_double,
            ct.c_int,
            ct.c_double,
            ct.c_double,
            ct.c_double,
            ct.c_double,
            ct.c_long
        ]
        self.PointToPoint.restype = ct.c_int

        # Get the Area method.
        self.Area = getattr(self.ilm_dll, 'Area')
        self.Area.argtypes = [
            ct.c_double,
            ct.c_double,
            ct.c_int,
            ct.c_int,
            ct.c_double,
            ct.c_double,
            ct.c_double,
            ct.c_int,
            ct.c_double,
            ct.c_double,
            ct.c_double,
            ct.c_double,
            ct.c_long
        ]

    # %%
    def __exit__(
        self,
        exc_type,
        exc_value,
        exc_traceback
    ):
        """Close the .dll."""
        ct.windll.kernel32.FreeLibrary(self.ilm_dll._handle)

    # %%
    def print_version(
        self,
    ):
        """Print ILM.dll Library version information."""
        print((
            "**** Irregular Lunar Model Library ******************\n"
            f"    .dll Version:      {self.version}\n"
            f"    .dll Compile Time: {self.compile_time}\n"
            f"*****************************************************"
        ))


# %% Run program.
if __name__ == "__main__":
    print("Running program: " + os.path.basename(__file__))
    ilm = ILM()
    ilm.print_version()
    print(dir(ilm))
    print("Program complete.\n")
