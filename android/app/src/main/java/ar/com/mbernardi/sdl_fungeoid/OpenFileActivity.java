package ar.com.mbernardi.sdl_fungeoid;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.EditText;
import android.widget.Button;
import android.net.Uri;

import ar.com.mbernardi.sdl_fungeoid.OpenFileActivity;

public class OpenFileActivity extends Activity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_openfile);

        final Button button = (Button) findViewById(R.id.button);
        button.setOnClickListener(new View.OnClickListener()  {

            public void onClick(View v) {
                Intent data = new Intent();
                String text = "Result...";
                data.setData(Uri.parse(text));
                setResult(RESULT_OK, data);
                finish();
            }
        });

    }


}
