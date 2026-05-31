package com.darkxcheat.loader.app;

import android.app.Activity;
import android.app.Application;
import android.content.Context;
import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.util.Log;

import com.darkxcheat.loader.Main;
import org.lsposed.lsparanoid.Obfuscate;

import top.niunaijun.blackbox.BlackBoxCore;
import top.niunaijun.blackbox.app.configuration.ClientConfiguration;

@Obfuscate
public class BoxApplication extends Application {

    private static final String TAG = "BoxApplication";

    // FIX PROBLEM 2:
    // static keyword — activity destroy hone pe reset nahi hoga
    // process level pe ek baar hi true hoga
    private static boolean startmod = false;

    @Override
    protected void attachBaseContext(Context base) {
        super.attachBaseContext(base);
        try {
            BlackBoxCore.get().doAttachBaseContext(base, new ClientConfiguration() {
                @Override
                public String getHostPackageName() {
                    return base.getPackageName();
                }
            });
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    @Override
    public void onCreate() {
        super.onCreate();
        try {
            BlackBoxCore.get().doCreate();
            addLifecycleCallback();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public void addLifecycleCallback() {
        try {
            BlackBoxCore.get().addAppLifecycleCallback(
                new top.niunaijun.blackbox.app.configuration.AppLifecycleCallback() {

                    @Override
                    public void beforeCreateApplication(String packageName,
                                                        String processName,
                                                        Context context,
                                                        int userId) {
                        Log.d(TAG, "beforeCreateApplication: " + packageName);
                    }

                    @Override
                    public void beforeApplicationOnCreate(String packageName,
                                                          String processName,
                                                          Application application,
                                                          int userId) {
                        Log.d(TAG, "beforeApplicationOnCreate: " + packageName);
                    }

                    @Override
                    public void afterApplicationOnCreate(String packageName,
                                                         String processName,
                                                         Application application,
                                                         int userId) {
                        Log.d(TAG, "afterApplicationOnCreate: " + packageName);
                        try {
                            application.registerActivityLifecycleCallbacks(
                                new ActivityLifecycleCallbacks() {

                                    @Override
                                    public void onActivityCreated(Activity activity,
                                                                  Bundle savedInstanceState) {}

                                    @Override
                                    public void onActivityStarted(Activity activity) {}

                                    @Override
                                    public void onActivityResumed(Activity activity) {
                                        // FIX PROBLEM 2 + 3:
                                        // startmod static hai — ek baar hi chalega
                                        // delay diya — libil2cpp.so mount hone ka wait
                                        if (!startmod) {
                                            startmod = true; // pehle set karo race condition avoid karne ke liye
                                            Log.d(TAG, "Scheduling Main.start() with delay...");

                                            // FIX PROBLEM 3: 3 second delay
                                            // libil2cpp.so BlackBox ke andar mount hone ka time deta hai
                                            new Handler(Looper.getMainLooper()).postDelayed(() -> {
                                                try {
                                                    Log.d(TAG, "Calling Main.start() now");
                                                    Main.start();
                                                    Log.d(TAG, "Main.start() success");
                                                } catch (Throwable e) {
                                                    Log.e(TAG, "Main.start() failed", e);
                                                    // Reset karo taaki dobara try ho sake
                                                    startmod = false;
                                                }
                                            }, 3000); // 3000ms = 3 seconds delay
                                        }
                                    }

                                    @Override
                                    public void onActivityPaused(Activity activity) {}

                                    @Override
                                    public void onActivityStopped(Activity activity) {}

                                    @Override
                                    public void onActivitySaveInstanceState(Activity activity,
                                                                            Bundle outState) {}

                                    @Override
                                    public void onActivityDestroyed(Activity activity) {
                                        // FIX PROBLEM 2:
                                        // startmod = false REMOVED
                                        // Lobby transition pe activity destroy hoti thi
                                        // aur startmod reset hota tha → crash
                                        // Ab startmod static hai → destroy pe reset nahi hoga
                                        Log.d(TAG, "onActivityDestroyed: " + activity.getClass().getSimpleName());
                                    }
                                });
                        } catch (Exception e) {
                            e.printStackTrace();
                        }
                    }
                });
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
