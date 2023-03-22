package org.lucar01.aes;

import org.junit.jupiter.api.Test;
import static org.junit.jupiter.api.Assertions.*;

import java.io.File;
import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.security.InvalidAlgorithmParameterException;
import java.security.InvalidKeyException;
import java.security.NoSuchAlgorithmException;
import java.security.spec.InvalidKeySpecException;

import javax.crypto.BadPaddingException;
import javax.crypto.IllegalBlockSizeException;
import javax.crypto.NoSuchPaddingException;

public class AESTest {

    final static String message = "Messaggio segreto";
    final static String plaintextFilePath = "plaintext.txt";
    final static String encryptedFilePath = "encrypted.txt";
    final static String decryptedFilePath = "decrypted.txt";

    @Test
    public void encryptDecryptTest() throws InvalidAlgorithmParameterException, NoSuchPaddingException, IllegalBlockSizeException, NoSuchAlgorithmException, InvalidKeySpecException, BadPaddingException, InvalidKeyException {
        final byte[] encryptedMessage = AES.encrypt(AESTest.message.getBytes(), AES.PASSWORD);

        assertNotNull(encryptedMessage);
        assertNotEquals(AESTest.message, new String(encryptedMessage, StandardCharsets.UTF_8));

        final byte[] decryptedMessage = AES.decrypt(encryptedMessage, AES.PASSWORD);
        final String clearMessage = new String(decryptedMessage, StandardCharsets.UTF_8);

        assertEquals(AESTest.message, clearMessage);
    }

    /*@Test //TODO: fix
    public void encryptDecryptFileTest() throws InvalidAlgorithmParameterException, NoSuchPaddingException, IllegalBlockSizeException, NoSuchAlgorithmException, InvalidKeySpecException, BadPaddingException, IOException, InvalidKeyException {

        final File file = new File(AESTest.plaintextFilePath);

        AES.encryptFile(AESTest.plaintextFilePath, AESTest.encryptedFilePath, AES.PASSWORD);
        final byte[] decryptedMessage = AES.decryptFile(AESTest.encryptedFilePath, AES.PASSWORD);
        final String clearMessage = new String(decryptedMessage, StandardCharsets.UTF_8);

        assertEquals(clearMessage, AESTest.message);
    }*/

}
