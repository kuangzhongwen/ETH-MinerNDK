package waterhole.miner.eth;

import android.app.Service;
import android.content.Context;
import android.content.Intent;
import android.os.IBinder;

import android.util.Log;
/**
 * 挖矿后台服务.
 */
public final class MineService extends Service {

    static {
        try {
            System.loadLibrary("eth-miner");
        } catch (Exception e) {
            Log.i("MineService", e.getMessage());
        }
    }

    public native void startJNIMine(StateObserver callback);

    private native void stopJNIMine();

    @Override
    public IBinder onBind(Intent intent) {
        return null;
    }

    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {
        startJNIMine(new StateObserver() {
            @Override
            public void onConnectPoolBegin() {
            }

            @Override
            public void onConnectPoolSuccess() {
            }

            @Override
            public void onConnectPoolFail(String error) {
            }

            @Override
            public void onPoolDisconnect(String error) {
            }

            @Override
            public void onMessageFromPool(String message) {
            }

            @Override
            public void onMiningError(String error) {
            }

            @Override
            public void onMiningStatus(double speed) {
            }
        });
        return START_STICKY;
    }

    @Override
    public void onDestroy() {
        super.onDestroy();

        stopJNIMine();
    }

    public static void startService(Context context) {
        if (context != null) {
            Intent intent = new Intent(context, MineService.class);
            context.startService(intent);
        }
    }

    public static void stopService(Context context) {
        if (context != null) {
            Intent intent = new Intent(context, MineService.class);
            context.stopService(intent);
        }
    }
}
