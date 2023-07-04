package com.android.car.zcrash;

import android.content.Context;
import android.os.Build;

public class NativeHandler {

    private static final String TAG = "zcrash";
    private static final NativeHandler instance = new NativeHandler();
    private long anrTimeoutMs = 25 * 1000;

    private Context ctx;
    private boolean crashRethrow;
    private ICrashCallback crashCallback;
    private boolean anrEnable;
    private boolean anrCheckProcessState;
    private ICrashCallback anrCallback;
    private ICrashCallback anrFastCallback;

    private boolean initNativeLibOk = false;

    void notifyJavaCrashed() {
        if (initNativeLibOk && anrEnable) {
            NativeHandler.nativeNotifyJavaCrashed();
        }
    }

    void testNativeCrash(boolean runInNewThread) {
        if (initNativeLibOk) {
            NativeHandler.nativeTestCrash(runInNewThread ? 1 : 0);
        }
    }

    private NativeHandler() {
    }

    int initialize(Context ctx,
                   ILibLoader libLoader,
                   String appId,
                   String appVersion,
                   String logDir,
                   boolean crashEnable,
                   boolean crashRethrow,
                   int crashLogcatSystemLines,
                   int crashLogcatEventsLines,
                   int crashLogcatMainLines,
                   boolean crashDumpElfHash,
                   boolean crashDumpMap,
                   boolean crashDumpFds,
                   boolean crashDumpNetworkInfo,
                   boolean crashDumpAllThreads,
                   int crashDumpAllThreadsCountMax,
                   String[] crashDumpAllThreadsWhiteList,
                   ICrashCallback crashCallback,
                   boolean anrEnable,
                   boolean anrRethrow,
                   boolean anrCheckProcessState,
                   int anrLogcatSystemLines,
                   int anrLogcatEventsLines,
                   int anrLogcatMainLines,
                   boolean anrDumpFds,
                   boolean anrDumpNetworkInfo,
                   ICrashCallback anrCallback,
                   ICrashCallback anrFastCallback) {
        if (libLoader == null) {
            try {
                System.loadLibrary("zcrash");
            } catch (Throwable e) {
                ZCrash.getLogger().e(Util.TAG, "NativeHandler System.loadLibrary failed", e);
                return Errno.LOAD_LIBRARY_FAILED;
            }
        } else {
            try {
                libLoader.loadLibrary("zcrash");
            } catch (Throwable e) {
                ZCrash.getLogger().e(Util.TAG, "NativeHandler ILibLoader.loadLibrary failed", e);
                return Errno.LOAD_LIBRARY_FAILED;
            }
        }

        this.ctx = ctx;
        this.crashRethrow = crashRethrow;
        this.crashCallback = crashCallback;
        this.anrEnable = anrEnable;
        this.anrCheckProcessState = anrCheckProcessState;
        this.anrCallback = anrCallback;
        this.anrFastCallback = anrFastCallback;
        this.anrTimeoutMs = anrRethrow ? 25 * 1000 : 45 * 1000; //setting rethrow to "false" is NOT recommended

        //init native lib
        try {
            int r = nativeInit(
                    Build.VERSION.SDK_INT,
                    Build.VERSION.RELEASE,
                    Util.getAbiList(),
                    Build.MANUFACTURER,
                    Build.BRAND,
                    Util.getMobileModel(),
                    Build.FINGERPRINT,
                    appId,
                    appVersion,
                    ctx.getApplicationInfo().nativeLibraryDir,
                    logDir,
                    crashEnable,
                    crashRethrow,
                    crashLogcatSystemLines,
                    crashLogcatEventsLines,
                    crashLogcatMainLines,
                    crashDumpElfHash,
                    crashDumpMap,
                    crashDumpFds,
                    crashDumpNetworkInfo,
                    crashDumpAllThreads,
                    crashDumpAllThreadsCountMax,
                    crashDumpAllThreadsWhiteList,
                    anrEnable,
                    anrRethrow,
                    anrLogcatSystemLines,
                    anrLogcatEventsLines,
                    anrLogcatMainLines,
                    anrDumpFds,
                    anrDumpNetworkInfo);
            if (r != 0) {
                ZCrash.getLogger().e(Util.TAG, "NativeHandler init failed");
                return Errno.INIT_LIBRARY_FAILED;
            }
            initNativeLibOk = true;
            return 0; //OK
        } catch (Throwable e) {
            ZCrash.getLogger().e(Util.TAG, "NativeHandler init failed", e);
            return Errno.INIT_LIBRARY_FAILED;
        }
    }

    static NativeHandler getInstance() {
        return instance;
    }

    public static native int nativeInit( int apiLevel,
                                         String osVersion,
                                         String abiList,
                                         String manufacturer,
                                         String brand,
                                         String model,
                                         String buildFingerprint,
                                         String appId,
                                         String appVersion,
                                         String appLibDir,
                                         String logDir,
                                         boolean crashEnable,
                                         boolean crashRethrow,
                                         int crashLogcatSystemLines,
                                         int crashLogcatEventsLines,
                                         int crashLogcatMainLines,
                                         boolean crashDumpElfHash,
                                         boolean crashDumpMap,
                                         boolean crashDumpFds,
                                         boolean crashDumpNetworkInfo,
                                         boolean crashDumpAllThreads,
                                         int crashDumpAllThreadsCountMax,
                                         String[] crashDumpAllThreadsWhiteList,
                                         boolean traceEnable,
                                         boolean traceRethrow,
                                         int traceLogcatSystemLines,
                                         int traceLogcatEventsLines,
                                         int traceLogcatMainLines,
                                         boolean traceDumpFds,
                                         boolean traceDumpNetworkInfo);

    private static native void nativeNotifyJavaCrashed();

    private static native void nativeTestCrash(int runInNewThread);
}
