import cv2
import pyttsx3
import time
import RPi.GPIO as GPIO

def initialize_tts():
    """Initialize text-to-speech engine."""
    tts_engine = pyttsx3.init()
    tts_engine.setProperty('rate', 150)  # Speed of speech
    tts_engine.setProperty('volume', 0.9)  # Volume level
    return tts_engine

def say_hello(tts_engine):
    """Use text-to-speech to say hello."""
    tts_engine.say("Hello! Nice to see you.")
    tts_engine.runAndWait()

def initialize_camera():
    """Initialize the webcam or Pi camera."""
    return cv2.VideoCapture(0)

def detect_face(frame, face_cascade):
    """Detect faces in a given frame."""
    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    faces = face_cascade.detectMultiScale(gray, scaleFactor=1.1, minNeighbors=5, minSize=(30, 30))
    return faces

def draw_bounding_box(frame, faces):
    """Draw a bounding box around each detected face."""
    for (x, y, w, h) in faces:
        cv2.rectangle(frame, (x, y), (x + w, y + h), (255, 0, 0), 2)  # Blue box with thickness of 2
    return frame

def main():
    face_cascade = cv2.CascadeClassifier(cv2.data.haarcascades + 'haarcascade_frontalface_default.xml')
    tts_engine = initialize_tts()
    camera = initialize_camera()

    last_detection_time = 0
    greeting_interval = 3  # Minimum interval between greetings in seconds
    
    GPIO.setmode(GPIO.BCM)  # Use BCM pin numbering
    GPIO.setwarnings(False)  # Ignore warnings
    LED_PIN = 24  # GPIO 18
    GPIO.setup(LED_PIN, GPIO.OUT)  # Set GPIO 18 as an output
    try:
        while True:
            ret, frame = camera.read()
            if not ret:
                print("Failed to capture image.")
                break

            faces = detect_face(frame, face_cascade)
            # GPIO.output(LED_PIN, GPIO.HIGH)
            # If a face is detected and enough time has passed, say hello
            #and time.time() - last_detection_time > greeting_interval
            if len(faces) > 0:
                GPIO.output(LED_PIN, GPIO.HIGH)
                say_hello(tts_engine)
                last_detection_time = time.time()
            else:
                GPIO.output(LED_PIN, GPIO.LOW)
            # Draw bounding boxes around detected faces
            frame = draw_bounding_box(frame, faces)

            # Display the frame with face rectangles for feedback
            cv2.imshow("Face Detection", frame)

            # Exit loop on pressing 'q'
            if cv2.waitKey(1) & 0xFF == ord('q'):
                GPIO.cleanup()
                break

    finally:
        camera.release()
        cv2.destroyAllWindows()

if __name__ == "__main__":
    main()
