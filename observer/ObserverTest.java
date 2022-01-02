package il.co.ilrd.observer;

import static org.junit.jupiter.api.Assertions.assertEquals;

import org.junit.jupiter.api.Test;

public class ObserverTest {
	@Test
	public void TestOneObserver() {
		Subject patient = new Subject();
		HealthObserver health = new HealthObserver();
		
		health.register(patient);
		patient.setBodyTemp(37.15);
		assertEquals(37.15, health.getPatientTemp());
		patient.setBodyTemp(38.0);
		assertEquals(38.0, health.getPatientTemp());
		health.unregister();
		patient.setBodyTemp(40.0);
		assertEquals(38.0, health.getPatientTemp());
		patient.terminate();
	}
	
	@Test
	public void TestSevsralObserver() {
		Subject patient = new Subject();
		HealthObserver health1 = new HealthObserver();
		HealthObserver health2 = new HealthObserver();
		BloodObserver nurse1 = new BloodObserver();
		BloodObserver nurse2 = new BloodObserver();
		BloodObserver nurse3 = new BloodObserver();
		
		health1.register(patient);
		health2.register(patient);
		nurse1.register(patient);
		
		patient.setBodyTemp(37.15);
		
		String bloodPreasure = "180 / 90";
		patient.setBlood(bloodPreasure);
		assertEquals(bloodPreasure, nurse1.getPatientBP());
		
		nurse2.register(patient);
		nurse3.register(patient);
		bloodPreasure = "200 / 110";
		patient.setBlood(bloodPreasure);
		assertEquals(bloodPreasure, nurse1.getPatientBP());
		assertEquals(bloodPreasure, nurse2.getPatientBP());
		assertEquals(bloodPreasure, nurse3.getPatientBP());
		
		nurse2.unregister();
		
		patient.setBodyTemp(40.0);
		assertEquals(40.0, health2.getPatientTemp());
		patient.terminate();
	}
	
}

class HealthObserver{
	private double patientTemp;
	private CallBack<Double> thermometer = new CallBack<>((temp)->{patientTemp = temp;}, ()->{System.out.println("thermo stoped");});
	
	
	public void register(Subject sub) {
		sub.registerBodyTemp(thermometer);
	}
	
	public void unregister() {
		thermometer.getDispatcher().unregister(thermometer);
	}

	public double getPatientTemp() {
		return patientTemp;
	}
}


class BloodObserver{
	private String patientBP;
	private CallBack<String> bpGauge = new CallBack<>((bp)->{patientBP = bp;}, ()->{System.out.println("bp stoped");});
	
	public void register(Subject sub) {
		sub.registerBlood(bpGauge);
	}
	
	public void unregister() {
		bpGauge.getDispatcher().unregister(bpGauge);
	}
	
	public String getPatientBP() {
		return patientBP;
	}
}

class Subject {
	private String bloodPreasure = "120 / 80";
	private double bodyTemp = 36.6;
	
	private Dispatcher<String> bloodDispetcher = new Dispatcher<>(); 
	private Dispatcher<Double> bodyDispetcher = new Dispatcher<>(); 

	public void registerBlood(CallBack<String> bpGauge) {
		bloodDispetcher.register(bpGauge);
	}
	
	public void registerBodyTemp(CallBack<Double> thermometer) {
		bodyDispetcher.register(thermometer);
	}
	
	public void terminate() {
		bloodDispetcher.terminate();
		bodyDispetcher.terminate();
	}
	
	public void setBlood(String bloodPreasure) {
		this.bloodPreasure = bloodPreasure;
		bloodDispetcher.notifyAll(this.bloodPreasure);
	}
	
	public void setBodyTemp(Double bodyTemp) {
		this.bodyTemp = bodyTemp;
		bodyDispetcher.notifyAll(this.bodyTemp);
	}

}