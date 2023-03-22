package org.lucar01.aes;

import javax.crypto.BadPaddingException;
import javax.crypto.IllegalBlockSizeException;
import javax.crypto.NoSuchPaddingException;
import java.nio.charset.StandardCharsets;
import java.security.InvalidAlgorithmParameterException;
import java.security.InvalidKeyException;
import java.security.NoSuchAlgorithmException;
import java.security.spec.InvalidKeySpecException;
import java.util.Arrays;

public class Main {
    public static void main(String[] args) throws InvalidAlgorithmParameterException, NoSuchPaddingException, IllegalBlockSizeException, NoSuchAlgorithmException, InvalidKeySpecException, BadPaddingException, InvalidKeyException {

        System.out.println("AES v1.0.0");

        final String message = "Messaggio segreto";
        System.out.println("message: " + message);

        final byte[] encryptedMessage = AES.encrypt(message.getBytes(), AES.PASSWORD);
        System.out.println("encryptedMessage: " + new String(encryptedMessage, StandardCharsets.UTF_8));

        final byte[] decryptedMessage = AES.decrypt(encryptedMessage, AES.PASSWORD);
        final String decryptedClearMessage = new String(decryptedMessage, StandardCharsets.UTF_8);
        System.out.println("decryptedClearMessage: " + decryptedClearMessage);
    }
}