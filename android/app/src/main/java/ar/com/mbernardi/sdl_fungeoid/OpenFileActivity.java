package ar.com.mbernardi.sdl_fungeoid;

import androidx.appcompat.app.AppCompatActivity;
import androidx.recyclerview.widget.RecyclerView;
import androidx.recyclerview.widget.LinearLayoutManager;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.view.ViewGroup;
import android.view.LayoutInflater;
import android.widget.TextView;
import android.widget.FrameLayout;
import android.widget.Button;
import android.net.Uri;

import ar.com.mbernardi.sdl_fungeoid.OpenFileActivity;

public class OpenFileActivity extends AppCompatActivity {

    private String[] filenames = {"sdfsdfds", "sdfsdfsd", "erwrwerwerw"};
    private RecyclerView recyclerView;
    private RecyclerView.Adapter openFileAdapter;
    private RecyclerView.LayoutManager layoutManager;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_openfile);

        // RecyclerView that holds list of files

        recyclerView = (RecyclerView) findViewById(R.id.files_recycler);

        // Improves performance as the list is fixed
        recyclerView.setHasFixedSize(true);

        layoutManager = new LinearLayoutManager(this);
        recyclerView.setLayoutManager(layoutManager);

        openFileAdapter = new FilenameAdapter(filenames);
        recyclerView.setAdapter(openFileAdapter);

        // Set up button

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

class FilenameAdapter extends RecyclerView.Adapter<FilenameAdapter.FilenameViewHolder> {
    private String[] filenameList;

    public FilenameAdapter(String[] filenames) {
        filenameList = filenames;
    }

    public static class FilenameViewHolder extends RecyclerView.ViewHolder {
        public TextView textView;

        public FilenameViewHolder(FrameLayout v) {
            super(v);
            textView = (TextView) v.findViewById(R.id.text);
        }
    }

    // Create new views
    @Override
    public FilenameAdapter.FilenameViewHolder onCreateViewHolder(ViewGroup parent,
            int viewType) {

        FrameLayout view = (FrameLayout) LayoutInflater.from(parent.getContext())
                .inflate(R.layout.filename_recycler_item, parent, false);

        FilenameViewHolder vh = new FilenameViewHolder(view);
        return vh;
    }

    // Replace view text
    @Override
    public void onBindViewHolder(FilenameViewHolder holder, int position) {
        holder.textView.setText(filenameList[position]);
    }

    @Override
    public int getItemCount() {
        return filenameList.length;
    }
}

