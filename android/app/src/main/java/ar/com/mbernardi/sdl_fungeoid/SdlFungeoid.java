package ar.com.mbernardi.sdl_fungeoid;

import android.widget.Toast;

import org.libsdl.app.SDLActivity;

public class SdlFungeoid extends SDLActivity {

    public void test() {

        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                Toast.makeText(getApplicationContext(), "hola", Toast.LENGTH_SHORT).show();
            }
        });
    }
}
