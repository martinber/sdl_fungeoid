package ar.com.mbernardi.sdl_fungeoid;

import android.content.Intent;
import android.os.Bundle;
import android.net.Uri;
import java.io.File;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

import ar.com.mbernardi.sdl_fungeoid.FileActivity;

public class SaveFileAsActivity extends FileActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        Button save_button = findViewById(R.id.save_button);
        save_button.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                Intent data = new Intent();
                TextView text_view = findViewById(R.id.filename_field);
                data.setData(Uri.parse(
                        getExternalFilesDir(null).getAbsolutePath()
                        + "/"
                        + text_view.getText()
                    ));
                setResult(RESULT_OK, data);
                finish();
            }
        });

    }

    @Override
    int getLayout() {
        return R.layout.activity_savefileas;
    }

    @Override
    public void itemClicked(File file) {
        TextView text_view = findViewById(R.id.filename_field);
        text_view.setText(file.getName());
    }
}
