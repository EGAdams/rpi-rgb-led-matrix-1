# Persona
Act as a world-class Android developer

# Task 1
Read the following main chat XML and Pay attention to what this line does:
```xml tools:listitem="@layout/chat_message_sent"></ListView> ```

# Source XML to Analyze
```xml
<?xml version="1.0" encoding="utf-8"?>
<LinearLayout xmlns:android="http://schemas.android.com/apk/res/android"
    xmlns:tools="http://schemas.android.com/tools"
    android:layout_width="match_parent"
    android:layout_height="match_parent"
    android:orientation="vertical">

    <ListView
        android:id="@+id/list"
        android:layout_width="wrap_content"
        android:layout_height="0dp"
        android:layout_weight="1"
        android:baselineAligned="false"
        android:divider="@color/white"
        android:dividerHeight="@dimen/pad_5dp"
        android:fastScrollEnabled="true"
        android:paddingTop="@dimen/pad_10dp"
        android:paddingBottom="@dimen/pad_5dp"
        tools:listitem="@layout/chat_message_sent"></ListView>

    <LinearLayout
        android:layout_width="match_parent"
        android:layout_height="wrap_content"
        android:background="@color/gray_light"
        android:gravity="center_vertical"
        android:padding="@dimen/pad_5dp"
        tools:context=".MainActivity">


        <EditText
            android:id="@+id/txt"
            style="@style/edittext_msg"
            android:layout_width="0dp"
            android:layout_height="wrap_content"
            android:layout_weight="1"
            android:hint="@string/type_msg"></EditText>

        <Button
            android:id="@+id/btnSend"
            android:layout_width="wrap_content"
            android:layout_height="wrap_content"
            android:background="@drawable/rounded_rectangle_blue"
            android:text="@string/send"
            android:textColor="#FFFFFF" />
    </LinearLayout>

    <TextView
        android:id="@+id/sendingTo"
        android:layout_width="match_parent"
        android:layout_height="wrap_content"
        android:layout_marginTop="-75dp"
        android:textStyle="normal|italic" />

</LinearLayout>
```

# Task 2
The folowing XML shows that the message section is aligning to the right of the screen in its own XML view, but in the activity XML above it does not show.  I want you to fix it so that the message section is aligned to the right of the screen in both XML views.

# Second message Source XML to Analyze
```xml
<?xml version="1.0" encoding="utf-8"?>
<androidx.constraintlayout.widget.ConstraintLayout xmlns:android="http://schemas.android.com/apk/res/android"
    xmlns:app="http://schemas.android.com/apk/res-auto"
    xmlns:tools="http://schemas.android.com/tools"
    android:layout_width="match_parent"
    android:layout_height="wrap_content"
    android:paddingTop="8dp"
    android:visibility="visible"
    tools:visibility="visible">

    <TextView
        android:id="@+id/email_name"
        android:layout_width="wrap_content"
        android:layout_height="wrap_content"
        android:layout_marginStart="24dp"
        android:layout_marginTop="8dp"
        android:layout_marginEnd="16dp"
        android:layout_marginBottom="8dp"
        android:text="John Doe"
        android:textSize="12sp"
        app:layout_constraintBottom_toTopOf="@+id/message"
        app:layout_constraintEnd_toStartOf="@+id/text_message_time"
        app:layout_constraintStart_toStartOf="parent"
        app:layout_constraintTop_toTopOf="parent" />

    <TextView
        android:id="@+id/message"
        android:layout_width="wrap_content"
        android:layout_height="wrap_content"
        android:layout_marginStart="16dp"
        android:layout_marginTop="24dp"
        android:layout_marginEnd="16dp"
        android:layout_marginBottom="16dp"
        android:background="@drawable/rounded_rectangle_orange"
        android:maxWidth="240dp"
        android:padding="8dp"
        android:text="hi man, how are you? We are going to need a longer example than this because hi man how are you isn't long enough"
        android:textColor="#ffffff"
        app:layout_constraintBottom_toBottomOf="parent"
        app:layout_constraintEnd_toEndOf="parent" 
    app:layout_constraintTop_toBottomOf="@+id/email_name"
    app:layout_constraintVertical_bias="0.0" />

    <TextView
        android:id="@+id/text_message_time"
        android:layout_width="wrap_content"
        android:layout_height="wrap_content"
        android:layout_marginStart="2dp"
        android:layout_marginTop="8dp"
        android:layout_marginEnd="16dp"
        android:layout_marginBottom="8dp"
        android:text="11:40"
        android:textSize="12sp"
        app:layout_constraintBottom_toTopOf="@+id/message"
        app:layout_constraintEnd_toEndOf="parent"
        app:layout_constraintStart_toEndOf="@+id/email_name"
        app:layout_constraintTop_toTopOf="parent" />

    <ImageView
        android:id="@+id/icon"
        android:layout_width="64dp"
        android:layout_height="64dp"
        android:layout_marginStart="16dp"
        android:layout_marginTop="16dp"
        android:contentDescription="@string/description_icon"
        app:layout_constraintStart_toStartOf="parent"
        app:layout_constraintTop_toTopOf="parent" />

    <TextView
        android:id="@+id/name"
        android:layout_width="0dp"
        android:layout_height="wrap_content"
        android:layout_marginStart="8dp"
        android:layout_marginTop="16dp"
        android:layout_marginEnd="8dp"
        android:maxLines="1"
        android:textAppearance="@style/TextAppearance.AppCompat.Large"
        app:layout_constraintStart_toEndOf="@+id/icon"
        app:layout_constraintEnd_toStartOf="@+id/text_message_time"
        app:layout_constraintTop_toTopOf="parent"
        tools:text="Cat" />

</androidx.constraintlayout.widget.ConstraintLayout>
```
