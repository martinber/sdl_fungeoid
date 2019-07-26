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

        // Set up RecyclerView list of files

        recyclerView = (RecyclerView) findViewById(R.id.files_recycler);

        // use this setting to improve performance if you know that changes
        // in content do not change the layout size of the RecyclerView
        recyclerView.setHasFixedSize(true);

        // use a linear layout manager
        layoutManager = new LinearLayoutManager(this);
        recyclerView.setLayoutManager(layoutManager);

        // specify an adapter (see also next example)
        openFileAdapter = new OpenFileAdapter(filenames);
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

class OpenFileAdapter extends RecyclerView.Adapter<OpenFileAdapter.FilenameViewHolder> {
    private String[] filenameList;

    public static class FilenameViewHolder extends RecyclerView.ViewHolder {
        // each data item is just a string in this case
        public TextView textView;
        public FilenameViewHolder(TextView v) {
            super(v);
            textView = v;
        }
    }

    // Provide a suitable constructor (depends on the kind of dataset)
    public OpenFileAdapter(String[] filenames) {
        filenameList = filenames;
    }

    // Create new views (invoked by the layout manager)
    @Override
    public OpenFileAdapter.FilenameViewHolder onCreateViewHolder(ViewGroup parent,
            int viewType) {
        // create a new view
        TextView v = (TextView) LayoutInflater.from(parent.getContext())
                .inflate(R.layout.filename_recycler_item, parent, false);

        FilenameViewHolder vh = new FilenameViewHolder(v);
        return vh;
    }

    // Replace the contents of a view (invoked by the layout manager)
    @Override
    public void onBindViewHolder(FilenameViewHolder holder, int position) {
        // - get element from your dataset at this position
        // - replace the contents of the view with that element
        holder.textView.setText(filenameList[position]);

    }

    // Return the size of your dataset (invoked by the layout manager)
    @Override
    public int getItemCount() {
        return filenameList.length;
    }
}

