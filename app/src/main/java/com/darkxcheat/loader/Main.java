package com.darkxcheat.loader;

import android.util.Log;

public class Main {
    
    private static final String TAG = "MagicLoader";

    static {
        try {
            System.loadLibrary("IlvMagic");
            Log.i(TAG, "Native library loaded");
        } catch (Throwable e) {
            Log.e(TAG, "Failed to load native library", e);
        }
    }

    public static native String getSupportedPackage();
    public static native String getSupportedVersion();
    public static native String getUpdateUrl();
    public static native String getTelegramUrl();
    public static native boolean checkVersionMatch(String installedVersion);
    public static native void checkCanRun(String packageName, String versionName);
    public static native void start();
}