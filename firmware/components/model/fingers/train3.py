import cv2
import mediapipe as mp

mp_hands = mp.solutions.hands
mp_draw = mp.solutions.drawing_utils

hands = mp_hands.Hands(
    static_image_mode=False,
    max_num_hands=2,
    min_detection_confidence=0.7,
    min_tracking_confidence=0.5
)

cap = cv2.VideoCapture(0)
FINGER_TIPS = [4, 8, 12, 16, 20]

while True:
    ret, frame = cap.read()
    if not ret:
        break

    frame = cv2.flip(frame, 1)  # mirror for natural webcam view
    h, w, _ = frame.shape
    rgb = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
    results = hands.process(rgb)

    if results.multi_hand_landmarks:
        for i, (hand_landmarks, handedness) in enumerate(zip(results.multi_hand_landmarks, results.multi_handedness)):
            hand_label = handedness.classification[0].label  # "Left" or "Right"
            mp_draw.draw_landmarks(frame, hand_landmarks, mp_hands.HAND_CONNECTIONS)
            lm = hand_landmarks.landmark
            fingers = []

            # Because the frame is flipped, invert thumb logic:
            if hand_label == "Right":  
                thumb_up = lm[FINGER_TIPS[0]].x < lm[FINGER_TIPS[0] - 1].x
            else:  
                thumb_up = lm[FINGER_TIPS[0]].x > lm[FINGER_TIPS[0] - 1].x

            fingers.append(1 if thumb_up else 0)

            for tip_id in FINGER_TIPS[1:]:
                fingers.append(1 if lm[tip_id].y < lm[tip_id - 2].y else 0)

            count = fingers.count(1)
            cv2.putText(frame, f"{hand_label} hand: {count}",
                        (10, 50 + i * 40), cv2.FONT_HERSHEY_SIMPLEX,
                        1, (0, 255, 0), 2)

    cv2.imshow("Finger Counter", frame)
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

cap.release()
cv2.destroyAllWindows()

