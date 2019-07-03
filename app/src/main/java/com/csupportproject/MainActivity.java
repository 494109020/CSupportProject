package com.csupportproject;

import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.content.pm.Signature;
import android.os.Bundle;
import android.os.Environment;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.View;
import android.widget.TextView;

import com.csupportproject.test.ThirdNdk;

import java.io.File;

/**
 * C++尝试,对C++不懂。只是看过一点点C。
 */
public class MainActivity extends AppCompatActivity {
    private static final String TAG = "cpp";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        TextView tv = (TextView) findViewById(R.id.sample_text);
        tv.setText(ThirdNdk.getString());
        String APK_dir = getApplicationContext().getFilesDir().getAbsolutePath()
                + "/wiwide/almightybussiness/download/";// 保存到app的包名路径下
        Log.e("TAG", APK_dir);
        APK_dir = Environment.getExternalStorageDirectory()
                .getAbsolutePath() + "/wiwide/almightybussiness/download/";
        Log.e("TAG", APK_dir);
        PackageManager pm = this.getPackageManager();
        PackageInfo pi = null;
        try {
            pi = pm.getPackageInfo(this.getPackageName(), PackageManager.GET_SIGNATURES);
            Signature[] signatures = pi.signatures;
            Signature signature0 = signatures[0];
            String sign = signature0.toCharsString();
            Log.e("TAG", sign);
        } catch (PackageManager.NameNotFoundException e) {
            e.printStackTrace();
        }
        tv.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                doBspatch();
            }
        });
    }

    private void doBspatch() {
        final File oldApk = new File(Environment.getExternalStorageDirectory(), "old.apk");
        final File destApk = new File(Environment.getExternalStorageDirectory(), "new.apk");
        final File patch = new File(Environment.getExternalStorageDirectory(), "old-to-new.patch");

        //一定要检查文件都存在

        BsPatch.bspatch(oldApk.getAbsolutePath(),
                destApk.getAbsolutePath(),
                patch.getAbsolutePath());

        if (destApk.exists())
            ApkExtract.install(this, destApk.getAbsolutePath());
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
//    public native String string();
//
//    public native String get();
//
//    public native int[] getInt();
}
