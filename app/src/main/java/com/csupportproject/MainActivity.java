package com.csupportproject;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.widget.TextView;

import com.csupportproject.test.JniClient;

import java.util.Arrays;

/**
 * C++尝试,对C++不懂。只是看过一点点C。
 */
public class MainActivity extends AppCompatActivity {
    private static final String TAG = "cpp";

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        System.currentTimeMillis();

        // Example of a call to a native method
        TextView tv = (TextView) findViewById(R.id.sample_text);
        tv.setText(JniClient.getStaticJniString());
        Log.i(TAG, Arrays.toString(getInt()));
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native String string();

    public native String get();

    public native int[] getInt();
}
